#include <WiFi.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "Comunidad";  // Enter SSID here
const char* password = "Comunidad3312";  //Enter Password here

/* Put IP Address details */
/*IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);*/

WebServer server(80);

char park0[1];
char park1[1];
char park2[1];
char park3[1];

int place0 = 0;
int place1 = 0;
int place2 = 0;
int place3 = 0;

int a = 25;
int b = 23;
int c = 26;
int d = 27;
int e = 33;
int f = 14;
int g = 22;

int cont = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  while(!Serial);

  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);

  /*WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);*/
  Serial.println("Try connecting to");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while(WiFi.status()!= WL_CONNECTED){
    delay(1000);
    Serial.println(".");
  }
  Serial.println("");
  Serial.println("WiFi conected success");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());
  
  server.on("/", handle_actualizar);
  server.on("/actualizar", handle_actualizar);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void handle_actualizar(){
  server.send(200, "text/html", SendHTML(place0, place1, place2, place3));
}

void loop() {
  server.handleClient();
  if(Serial2.available()>0){
    if(Serial2.read() == 'x'){
      Serial2.readBytesUntil(10, park0, 1);
      Serial2.readBytesUntil(10, park1, 1);
      Serial2.readBytesUntil(10, park2, 1);
      Serial2.readBytesUntil(10, park3, 1);
      if(park0[0] == '0'){
        place0 = 0;
      }else{
        place0 = 1;
      }
      if(park1[0] == '0'){
        place1 = 0;
      }else{
        place1 = 1;
      }
      if(park2[0] == '0'){
        place2 = 0;
      }else{
        place2 = 1;
      }
      if(park3[0] == '0'){
        place3 = 0;
      }else{
        place3 = 1;
      }
      Serial.print(place0);
      Serial.print(place1);
      Serial.print(place2);
      Serial.println(place3);
      cont = place0 + place1 + place2 + place3;
      if (cont == 0){
        digitalWrite(a, LOW);
        digitalWrite(b, LOW);
        digitalWrite(c, LOW);
        digitalWrite(d, LOW);
        digitalWrite(e, LOW);
        digitalWrite(f, LOW);
        digitalWrite(g, HIGH);
      }else if(cont == 1){
        digitalWrite(a, HIGH);
        digitalWrite(b, LOW);
        digitalWrite(c, LOW);
        digitalWrite(d, HIGH);
        digitalWrite(e, HIGH);
        digitalWrite(f, HIGH);
        digitalWrite(g, HIGH);
      }else if(cont == 2){
        digitalWrite(a, LOW);
        digitalWrite(b, LOW);
        digitalWrite(c, HIGH);
        digitalWrite(d, LOW);
        digitalWrite(e, LOW);
        digitalWrite(f, HIGH);
        digitalWrite(g, LOW);
      }else if(cont == 3){
        digitalWrite(a, LOW);
        digitalWrite(b, LOW);
        digitalWrite(c, LOW);
        digitalWrite(d, LOW);
        digitalWrite(e, HIGH);
        digitalWrite(f, HIGH);
        digitalWrite(g, LOW);
      }else if(cont == 4){
        digitalWrite(a, HIGH);
        digitalWrite(b, LOW);
        digitalWrite(c, LOW);
        digitalWrite(d, HIGH);
        digitalWrite(e, HIGH);
        digitalWrite(f, LOW);
        digitalWrite(g, LOW);
      }
    }
  }
}

String SendHTML(int a, int b, int c, int d){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr+="<head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr+="<!-- Bootstrap CSS --><link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css\"integrity=\"sha384-9aIt2nRpC12Uk9gS9baDl411NQApFmC26EwAOH8WgZl5MYYxFfc+NcPb1dKGj7Sk\" crossorigin=\"anonymous\">\n";
  ptr+="<title>Proyecto #3</title><script>function autoRefresh() {window.location = window.location.href;}setInterval('autoRefresh()', 2000);</script><style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr+="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}.button {display: block;width: 160px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 10px;text-align: center;vertical-align: middle;}.button-on {background-color: #3498db;}\n";
  ptr+=".button-on:active {background-color: #2980b9;}.button-off {background-color: #34495e;}.button-off:active {background-color: #2c3e50;}p {font-size: 14px;color: #888;margin-bottom: 10px;}th, td {border-radius: 10px;text-align: center;vertical-align: middle;}</style></head>\n";
  ptr+="<body><h1>Parking control &#128680 </h1><br><a class=\"button button-off\" href=\"actualizar\">Update</a><br><table class=\"table\"; border-radius: 4px><tbody><tr><th scope=\"row\">1</th><th scope=\"row\">2</th><th scope=\"row\">3</th><th scope=\"row\">4</th></tr><tr>\n";
  if(a == 0){
    ptr+="<td class=\"table-danger\">Taken &#128721</td>\n";
  }else{
    ptr+="<td class=\"table-primary\">Available &#128665</td>\n";
  }
  if(b == 0){
    ptr+="<td class=\"table-danger\">Taken &#128721</td>\n";
  }else{
    ptr+="<td class=\"table-primary\">Available &#128665</td>\n";
  }
  if(c == 0){
    ptr+="<td class=\"table-danger\">Taken &#128721</td>\n";
  }else{
    ptr+="<td class=\"table-primary\">Available &#128665</td>\n";
  }
  if(d == 0){
    ptr+="<td class=\"table-danger\">Taken &#128721</td>\n";
  }else{
    ptr+="<td class=\"table-primary\">Available &#128665</td>\n";
  }
  ptr+="</tr></tbody></table><!-- Optional JavaScript --><!-- jQuery first, then Popper.js, then Bootstrap JS --><script src=\"https://code.jquery.com/jquery-3.5.1.slim.min.js\"\n";
  ptr+="integrity=\"sha384-DfXdz2htPH0lsSSs5nCTpuj/zy4C+OGpamoFVy38MVBnE+IbbVYUew+OrCXaRkfj\"crossorigin=\"anonymous\"></script><script src=\"https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js\"integrity=\"sha384-Q6E9RHvbIyZFJoft+2mJbHaEWldlvI9IOYy5n3zV9zzTtmI3UksdQRVvoxMfooAo\"crossorigin=\"anonymous\"></script>\n";
  ptr+="<script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/js/bootstrap.min.js\"integrity=\"sha384-OgVRvuATP1z7JjHLkuOU7Xw704+h835Lr+6QL9UvYjZE3Ipu6Tp75j7Bh/kR0JKI\"crossorigin=\"anonymous\"></script></body></html>\n";
  return ptr;
}
