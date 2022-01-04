#include <ArduinoJson.h>

static char recv_buf[512];
static int led = 2;

static int at_send_check_response(char *p_ack, int timeout_ms, char *p_cmd, ...)
{
    int ch;
    int num = 0;
    int index = 0;
    int startMillis = 0;
    va_list args;
    memset(recv_buf, 0, sizeof(recv_buf));
    va_start(args, p_cmd);
    Serial2.print(p_cmd);
    Serial.print(p_cmd);
    va_end(args);
    delay(200);
    startMillis = millis();
 
    if (p_ack == NULL)
        return 0;
 
    do
    {
        while (Serial2.available() > 0)
        {
            ch = Serial2.read();
            recv_buf[index++] = ch;
            Serial.print((char)ch);
            delay(2);
        }
 
        if (strstr(recv_buf, p_ack) != NULL)
            return 1;
 
    } while (millis() - startMillis < timeout_ms);
    Serial.println();
    return 0;
}
 
static void recv_prase(char *p_msg)
{
    if (p_msg == NULL)
    {
      Serial.println("Received null");
        return;
    }
    char *p_start = NULL;
    char data[128];       // To hold the received bytes as characters
    
    int bytes_len=0;
    p_start = strstr(p_msg, "RX");
    if (p_start && (1 == sscanf(p_start, "RX \"%s\"", &data)))
    {
      for (int i=0; i<sizeof(data); i++) {
          if(int(data[i+1])==0) {
            bytes_len = i;
            break;
          }
    }

    // Convert the characters to a byteArray
      int message_len = bytes_len/2+1;
      byte out[message_len];
      auto getNum = [](char c){ return c > '9' ? c - 'A' + 10 : c - '0'; };
      for (int x=0, y=0; x<bytes_len; ++x, ++y)
        out[y] = (getNum(data[x++]) << 4) + getNum(data[x]);
      out[message_len] = '\0';

      // Print the received bytes
      for (int i=0; i<sizeof(out)-1; i++)
        Serial.println(out[i], HEX);
      
    }
}
 
void setup(void)
{
    Serial.begin(9600);
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    Serial2.begin(9600);
    Serial.print("Serial2 LOCAL TEST\r\n");
    at_send_check_response("+AT: OK", 100, "AT\r\n");
    at_send_check_response("+ID: AppEui", 1000, "AT+ID\r\n");
    at_send_check_response("+MODE: TEST", 1000, "AT+MODE=TEST\r\n");
    at_send_check_response("+CH: NUM, 1-2", 1000, "AT+CH=NUM,0-2\r\n");
    at_send_check_response("+LOG: QUIET", 1000, "AT+LOG=QUIET\r\n");
    at_send_check_response("+TEST: RXLRPKT", 5000, "AT+TEST=RXLRPKT\r\n");
    delay(200);
    digitalWrite(led, HIGH);
}
 
void loop(void)
{
      char cmd[128];
      // Transmit HEX Value
      sprintf(cmd, "");
      int ret = at_send_check_response("+TEST: RX", 1000, "");
      if (ret)
        recv_prase(recv_buf);
      else
        Serial.println("Receive failed!\r\n\r\n");
      delay(5000);
      
}
