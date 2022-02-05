# ESP32+Xiaomi Flower Care BLE통신 + Node-Red
![(Flower Care)노드레드](https://user-images.githubusercontent.com/98401825/152643790-b19af6ab-32f6-4df0-a44e-1d6d81db4f65.png)

## 준비물
---
1. Flower Care와 연동한 ESP32 DEVKIT V1 1개
2. Node-red

## 동작원리
---
1. 1-42-1 에서 Flower Care와 연동한 ESP32를 Node-Red와 연동 
2. Arduino에서 Json방식으로 데이터를 전송
3. 수신한 데이터들 중 필요한 값 산출
4. Node-red에서 출력 

* Node-red FLOW
```
[{"id":"d38549c569047972","type":"tab","label":"플로우 1","disabled":false,"info":"","env":[]},{"id":"d3b401cdda5304c1","type":"switch","z":"d38549c569047972","name":"","property":"payload.name","propertyType":"msg","rules":[{"t":"eq","v":"Temperature","vt":"str"},{"t":"eq","v":"Moisture","vt":"str"},{"t":"eq","v":"Light","vt":"str"},{"t":"eq","v":"Conductivity","vt":"str"}],"checkall":"true","repair":false,"outputs":4,"x":550,"y":100,"wires":[["32b609ccba2d062f"],["413402fce7e38bde"],["c3f73f78752f3243"],["4d24adeeb29a149b"]]},{"id":"32b609ccba2d062f","type":"debug","z":"d38549c569047972","name":"Temperature","active":true,"tosidebar":true,"console":false,"tostatus":false,"complete":"payload.value","targetType":"msg","statusVal":"","statusType":"auto","x":730,"y":40,"wires":[]},{"id":"0a8cd3c386e789e5","type":"json","z":"d38549c569047972","name":"","property":"payload","action":"","pretty":false,"x":410,"y":100,"wires":[["d3b401cdda5304c1"]]},{"id":"413402fce7e38bde","type":"debug","z":"d38549c569047972","name":"Moisture","active":true,"tosidebar":true,"console":false,"tostatus":false,"complete":"payload.value","targetType":"msg","statusVal":"","statusType":"auto","x":720,"y":80,"wires":[]},{"id":"c3f73f78752f3243","type":"debug","z":"d38549c569047972","name":"Light","active":true,"tosidebar":true,"console":false,"tostatus":false,"complete":"payload","targetType":"msg","statusVal":"","statusType":"auto","x":710,"y":120,"wires":[]},{"id":"4d24adeeb29a149b","type":"debug","z":"d38549c569047972","name":"Conductivity","active":true,"tosidebar":true,"console":false,"tostatus":false,"complete":"payload","targetType":"msg","statusVal":"","statusType":"auto","x":730,"y":160,"wires":[]},{"id":"9a0b20d876ad3744","type":"switch","z":"d38549c569047972","name":"","property":"payload","propertyType":"msg","rules":[{"t":"cont","v":"Temperature","vt":"str"},{"t":"cont","v":"Moisture","vt":"str"},{"t":"cont","v":"Light","vt":"str"},{"t":"cont","v":"Conductivity","vt":"str"}],"checkall":"true","repair":false,"outputs":4,"x":210,"y":100,"wires":[["0a8cd3c386e789e5"],["0a8cd3c386e789e5"],["0a8cd3c386e789e5"],["0a8cd3c386e789e5"]]},{"id":"e5cf5aea1054693c","type":"serial in","z":"d38549c569047972","name":"","serial":"8ddfb3c144951379","x":60,"y":100,"wires":[["9a0b20d876ad3744"]]},{"id":"8ddfb3c144951379","type":"serial-port","serialport":"COM8","serialbaud":"115200","databits":"8","parity":"none","stopbits":"1","waitfor":"","dtr":"none","rts":"none","cts":"none","dsr":"none","newline":"\\n","bin":"false","out":"char","addchar":"","responsetimeout":"10000"}]
```

![Flower Care Node-red-flow](https://user-images.githubusercontent.com/98401825/152645423-a303a225-74d7-4d51-8a90-1d98edd56c78.png)


