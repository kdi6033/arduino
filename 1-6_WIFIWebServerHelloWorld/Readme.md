키우미센서
======================
![센서-015](https://user-images.githubusercontent.com/83691399/122887584-04828480-d37c-11eb-9c5f-77ed88931cab.png)

## 1.1 키우미센서란?
키우미센서는 에코나레에서 개발한 온도, 습도, CO2를 측정해주는 센서입니다.

## 1.2 키우미센서 프로토콜
>
보드레이트 : 9600   
문자데이타로 입력하세요  
주소확인    :FFFB0000000006\r\n   
온도요청	:010400010001F9\r\n   
습도요청	:010400020001F8\r\n   
CO2요청 :010400030001F7\r\n   
온도,습도요청	:010400010002F8\r\n   
습도,CO2요청	:010400020002F7\r\n   
온도,습도,CO2요청	:010400010003F7\r\n   

## 1.3 키우미센서 배선도
![1111](https://user-images.githubusercontent.com/37902752/130716248-827ba40e-9641-435c-b018-999aec72e095.png)

## 1.4 노드레드 소스 코드
노드레드 소스 코드는 상단 ino파일을 참고 하세요~!!

```
[{"id":"4053fcb0.8c9804","type":"serial out","z":"ace6987b.29b6f8","name":"","serial":"adc9d8bd.2b3c18","x":580,"y":300,"wires":[]},{"id":"4ac092bc.0fbb2c","type":"serial in","z":"ace6987b.29b6f8","name":"","serial":"adc9d8bd.2b3c18","x":590,"y":400,"wires":[["2af17914.bbc146","d91cb857.0667e8"]]},{"id":"2af17914.bbc146","type":"debug","z":"ace6987b.29b6f8","name":"","active":true,"tosidebar":true,"console":false,"tostatus":false,"complete":"false","statusVal":"","statusType":"auto","x":890,"y":260,"wires":[]},{"id":"63ee301a.847a8","type":"inject","z":"ace6987b.29b6f8","name":"온도 습도 CO2","props":[{"p":"payload"},{"p":"topic","vt":"str"}],"repeat":"","crontab":"","once":false,"onceDelay":0.1,"topic":"","payload":":010400010003F7","payloadType":"str","x":340,"y":260,"wires":[["4053fcb0.8c9804"]]},{"id":"f12dffcd.e9a3f","type":"ui_gauge","z":"ace6987b.29b6f8","name":"","group":"199b0928.389a87","order":2,"width":"3","height":"3","gtype":"wave","title":"CO2","label":"ph","format":"{{value}}","min":"400","max":"3000","colors":["#00b500","#e6e600","#ca3838"],"seg1":"600","seg2":"800","x":1010,"y":460,"wires":[]},{"id":"afd62bf0.242688","type":"ui_gauge","z":"ace6987b.29b6f8","name":"","group":"199b0928.389a87","order":0,"width":"3","height":"3","gtype":"donut","title":"온도","label":"℃","format":"{{value}}","min":"-20","max":"70","colors":["#50e2e0","#4bf00f","#ca3838"],"seg1":"20","seg2":"35","x":1010,"y":340,"wires":[]},{"id":"684da238.30dc0c","type":"ui_gauge","z":"ace6987b.29b6f8","name":"","group":"199b0928.389a87","order":1,"width":"3","height":"3","gtype":"wave","title":"습도","label":"%","format":"{{value}}","min":0,"max":"100","colors":["#00b500","#e6e600","#ca3838"],"seg1":"40","seg2":"80","x":1010,"y":400,"wires":[]},{"id":"d91cb857.0667e8","type":"function","z":"ace6987b.29b6f8","name":"","func":"var msg1={};\nvar msg2={};\nvar msg3={};\nmsg1.payload = msg.payload.substr(7,4);\nmsg2.payload = msg.payload.substr(11,4);\nmsg3.payload = msg.payload.substr(15,4);\nmsg1.payload = parseInt(\"0x\"+msg1.payload-200) / 10.0;\nmsg2.payload = parseInt(\"0x\"+msg2.payload) / 10.0;\nmsg3.payload = parseInt(\"0x\"+msg3.payload);\nreturn [msg1,msg2,msg3];","outputs":3,"noerr":0,"initialize":"","finalize":"","x":800,"y":420,"wires":[["afd62bf0.242688"],["684da238.30dc0c"],["f12dffcd.e9a3f"]]},{"id":"1ea392e7.eb90fd","type":"inject","z":"ace6987b.29b6f8","name":"온도 습도 CO2","props":[{"p":"payload"},{"p":"topic","vt":"str"}],"repeat":"","crontab":"","once":false,"onceDelay":0.1,"topic":"","payload":":01040601B102470305F2","payloadType":"str","x":600,"y":520,"wires":[["d91cb857.0667e8"]]},{"id":"adc9d8bd.2b3c18","type":"serial-port","serialport":"COM11","serialbaud":"9600","databits":"8","parity":"none","stopbits":"1","waitfor":"","dtr":"none","rts":"none","cts":"none","dsr":"none","newline":"\\n","bin":"false","out":"char","addchar":"\\r\\n","responsetimeout":"10000"},{"id":"199b0928.389a87","type":"ui_group","name":"키우미","tab":"5eec05a4.959bac","order":1,"disp":true,"width":"6","collapse":false},{"id":"5eec05a4.959bac","type":"ui_tab","name":"키우미","icon":"dashboard","order":3,"disabled":false,"hidden":false}]
```
