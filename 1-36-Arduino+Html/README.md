## 아두이노 HTML 웹페이지 만들기
아두이노에서 웹페이지 html을 만드는 3가지 방법을 소개 합니다.      
자기에게 편리한 것을 사용하세요.

예제 html 파일은 script가 포함된 아래 사이트의 파일을 사용 했습니다.      
html 소스프로그램 : https://www.w3schools.com/jsref/tryit.asp?filename=tryjsref_onclick

## 방법 1
html을 작성하기 쉬운 방법이다.
참조유튜브 : https://www.youtube.com/watch?v=lyoBWH92svk

```
const char webpage1[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <body>
    <h1>The onclick Event 01</h1>
    <p>The onclick event is used to trigger a function when an element is clicked on.</p>
    <p>Click the button to trigger a function that will output "Hello World" in a p element with id="demo".</p>
    <button onclick="myFunction()">Click me</button>
    <p id="demo"></p>
    <script>
    function myFunction() {
      document.getElementById("demo").innerHTML = "Hello World";
    }
    </script>
  </body>
</html>
)=====";
void handleRoot() {
  server.send(200, "text/html", webpage1);
}
```

## 방법 2
html 형식으로 작성된 파일을 아두이노가 인식할 수 있는 문장으로 변환하여 사용한다. 변환은 아래 사이트를 이용하면 된다.    
참조유튜브 : https://www.youtube.com/watch?v=FyPZ29wgD7Y      
text 변환 사이트 : http://davidjwatts.com/youtube/esp8266/esp-convertHTM.html#       
 https://tomeko.net/online_tools/cpp_text_escape.php?lang=en     

```
const char webpage2[] PROGMEM={"<!DOCTYPE html> <html> <body> <h1>The onclick Event 02</h1> <p>The onclick event is used to trigger a function when an element is clicked on.</p> <p>Click the button to trigger a function that will output \"Hello World\" in a p element with id=\"demo\".</p> <button onclick=\"myFunction()\">Click me</button> <p id=\"demo\"></p> <script> function myFunction() { document.getElementById(\"demo\").innerHTML = \"Hello World\"; } </script> </body> </html>"};

void handleRoot() {
  server.send(200, "text/html", webpage2);
}

```

## 방법 3 
string으로 html을 작성한다 본인이 사용하는 방법으로 프로그램에서 사용하는 변수를 처리하기 위해 별도로 script를 작성할 필요가 없다.
변환은 위 변환 사이트를 이용한다.
```
String webpage3="<!DOCTYPE html> <html> <body> <h1>The onclick Event 03</h1> <p>The onclick event is used to trigger a function when an element is clicked on.</p> <p>Click the button to trigger a function that will output \"Hello World\" in a p element with id=\"demo\".</p> <button onclick=\"myFunction()\">Click me</button> <p id=\"demo\"></p> <script> function myFunction() { document.getElementById(\"demo\").innerHTML = \"Hello World\"; } </script> </body> </html>";

void handleRoot() {
  server.send(200, "text/html", webpage3);
}
```





