# Node-RED를 이용한 마스크 감별기
![제목을-입력해주세요_-001 (1)](https://user-images.githubusercontent.com/83691399/158743846-fad2f6a2-63cd-411c-8374-ba337e6f52a9.png)
Teachable Machine을 학습시켜 Node-RED로 불러오는 과정을 설명 드리겠습니다.

[유튜브 링크] ()
## ESP32 Cam 소스 코드링크

https://github.com/kdi6033/arduino/blob/master/1-40-1%20ESP32-CAM%20-%20AI%20tensorflow/Esp32Cam/Esp32Cam.ino

ESP32 Arduino - AI Thinker ESP32-Cam

## Teachable Machine 
우선 학습 시킬 사진을 여러장 모아줍니다. 예시는 많을수록좋습니다. 


## Node-Red
```   
[
    {
        "id": "8544f2512bce6cb3",
        "type": "mqtt in",
        "z": "8693e2d2fcae0e3b",
        "name": "",
        "topic": "/picture",
        "qos": "0",
        "datatype": "auto",
        "broker": "8f4f6e74.2997a",
        "nl": false,
        "rap": false,
        "inputs": 0,
        "x": 150,
        "y": 320,
        "wires": [
            [
                "95cb192ed87b0437",
                "37efadef88511b4f"
            ]
        ]
    },
    {
        "id": "2d8108c49942d977",
        "type": "mqtt out",
        "z": "8693e2d2fcae0e3b",
        "name": "",
        "topic": "TakeAPicture",
        "qos": "0",
        "retain": "false",
        "respTopic": "",
        "contentType": "",
        "userProps": "",
        "correl": "",
        "expiry": "",
        "broker": "8f4f6e74.2997a",
        "x": 570,
        "y": 180,
        "wires": []
    },
    {
        "id": "757168d9982fe3ee",
        "type": "mqtt out",
        "z": "8693e2d2fcae0e3b",
        "name": "",
        "topic": "JSONConfig",
        "qos": "0",
        "retain": "false",
        "respTopic": "",
        "contentType": "",
        "userProps": "",
        "correl": "",
        "expiry": "",
        "broker": "8f4f6e74.2997a",
        "x": 550,
        "y": 120,
        "wires": []
    },
    {
        "id": "5cc88ba4ae10c79e",
        "type": "inject",
        "z": "8693e2d2fcae0e3b",
        "name": "",
        "props": [
            {
                "p": "payload",
                "v": "",
                "vt": "date"
            },
            {
                "p": "topic",
                "v": "",
                "vt": "string"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payload": "",
        "payloadType": "date",
        "x": 380,
        "y": 120,
        "wires": [
            [
                "757168d9982fe3ee"
            ]
        ]
    },
    {
        "id": "9314cee0955dc958",
        "type": "ui_button",
        "z": "8693e2d2fcae0e3b",
        "name": "",
        "group": "a6c9238f876481dc",
        "order": 2,
        "width": 0,
        "height": 0,
        "passthru": false,
        "label": "사진찍기",
        "tooltip": "",
        "color": "",
        "bgcolor": "",
        "className": "",
        "icon": "",
        "payload": "",
        "payloadType": "str",
        "topic": "",
        "topicType": "str",
        "x": 360,
        "y": 180,
        "wires": [
            [
                "2d8108c49942d977"
            ]
        ]
    },
    {
        "id": "647fa6bb0836eff4",
        "type": "file",
        "z": "8693e2d2fcae0e3b",
        "name": "",
        "filename": "d:/test.jpeg",
        "appendNewline": false,
        "createDir": false,
        "overwriteFile": "true",
        "encoding": "base64",
        "x": 630,
        "y": 380,
        "wires": [
            [
                "8d5f1845a84f4285"
            ]
        ]
    },
    {
        "id": "d7608c424d63ebf1",
        "type": "file",
        "z": "8693e2d2fcae0e3b",
        "name": "",
        "filename": "d:/test.txt",
        "appendNewline": false,
        "createDir": false,
        "overwriteFile": "true",
        "encoding": "ascii",
        "x": 980,
        "y": 320,
        "wires": [
            []
        ]
    },
    {
        "id": "37efadef88511b4f",
        "type": "base64",
        "z": "8693e2d2fcae0e3b",
        "name": "",
        "action": "",
        "property": "payload",
        "x": 500,
        "y": 320,
        "wires": [
            [
                "d7608c424d63ebf1",
                "647fa6bb0836eff4",
                "6d9b734becb88153"
            ]
        ]
    },
    {
        "id": "95cb192ed87b0437",
        "type": "image",
        "z": "8693e2d2fcae0e3b",
        "name": "",
        "width": 160,
        "data": "payload",
        "dataType": "msg",
        "thumbnail": false,
        "active": true,
        "pass": false,
        "outputs": 0,
        "x": 1080,
        "y": 160,
        "wires": []
    },
    {
        "id": "b9374d907df2c45b",
        "type": "teachable machine",
        "z": "8693e2d2fcae0e3b",
        "name": "분석기",
        "mode": "online",
        "modelUrl": "https://teachablemachine.withgoogle.com/models/LEm8AEX7r/",
        "localModel": "teachable_model",
        "output": "best",
        "activeThreshold": false,
        "threshold": 80,
        "activeMaxResults": false,
        "maxResults": 3,
        "passThrough": true,
        "x": 890,
        "y": 480,
        "wires": [
            [
                "3ddf6fc8bdee440b",
                "fcd2b516407590ee"
            ]
        ]
    },
    {
        "id": "8d5f1845a84f4285",
        "type": "file in",
        "z": "8693e2d2fcae0e3b",
        "name": "",
        "filename": "d:/test.jpeg",
        "format": "",
        "chunk": false,
        "sendError": false,
        "encoding": "none",
        "allProps": false,
        "x": 610,
        "y": 480,
        "wires": [
            [
                "b9374d907df2c45b"
            ]
        ]
    },
    {
        "id": "be8ef9a386c7acba",
        "type": "function",
        "z": "8693e2d2fcae0e3b",
        "name": "",
        "func": "if(msg.payload==\"마스크 X\")\nmsg.payload = \"마스크를 껴주세요!!\";\nelse\nmsg.payload = \"\";\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1380,
        "y": 480,
        "wires": [
            [
                "ca0470577f5daa93"
            ]
        ]
    },
    {
        "id": "3ddf6fc8bdee440b",
        "type": "change",
        "z": "8693e2d2fcae0e3b",
        "name": "",
        "rules": [
            {
                "t": "set",
                "p": "payload",
                "pt": "msg",
                "to": "payload.0.class",
                "tot": "msg"
            }
        ],
        "action": "",
        "property": "",
        "from": "",
        "to": "",
        "reg": false,
        "x": 1140,
        "y": 460,
        "wires": [
            [
                "be8ef9a386c7acba"
            ]
        ]
    },
    {
        "id": "ca0470577f5daa93",
        "type": "debug",
        "z": "8693e2d2fcae0e3b",
        "name": "",
        "active": true,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "false",
        "statusVal": "",
        "statusType": "auto",
        "x": 1550,
        "y": 480,
        "wires": []
    },
    {
        "id": "f671ce1b16a90ec8",
        "type": "inject",
        "z": "8693e2d2fcae0e3b",
        "name": "",
        "props": [
            {
                "p": "payload",
                "v": "",
                "vt": "date"
            },
            {
                "p": "topic",
                "v": "",
                "vt": "string"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payload": "",
        "payloadType": "date",
        "x": 360,
        "y": 480,
        "wires": [
            [
                "8d5f1845a84f4285"
            ]
        ]
    },
    {
        "id": "fcd2b516407590ee",
        "type": "image",
        "z": "8693e2d2fcae0e3b",
        "name": "",
        "width": 160,
        "data": "image",
        "dataType": "msg",
        "thumbnail": false,
        "active": true,
        "pass": false,
        "outputs": 0,
        "x": 1380,
        "y": 340,
        "wires": []
    },
    {
        "id": "6d9b734becb88153",
        "type": "ui_template",
        "z": "8693e2d2fcae0e3b",
        "group": "a6c9238f876481dc",
        "name": "사진 출력",
        "order": 2,
        "width": "6",
        "height": "6",
        "format": "<img alt=\"HTTP Snap\" src=\"data:image/jpg;base64,{{msg.payload}}\" />\n",
        "storeOutMessages": true,
        "fwdInMessages": true,
        "resendOnRefresh": false,
        "templateScope": "local",
        "className": "",
        "x": 960,
        "y": 260,
        "wires": [
            []
        ]
    },
    {
        "id": "d025386088072c00",
        "type": "inject",
        "z": "8693e2d2fcae0e3b",
        "name": "",
        "props": [
            {
                "p": "payload"
            },
            {
                "p": "topic",
                "vt": "str"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payload": "",
        "payloadType": "date",
        "x": 360,
        "y": 240,
        "wires": [
            [
                "2d8108c49942d977"
            ]
        ]
    },
    {
        "id": "8f4f6e74.2997a",
        "type": "mqtt-broker",
        "name": "",
        "broker": "broker.mqtt-dashboard.com",
        "port": "1883",
        "clientid": "",
        "autoConnect": true,
        "usetls": false,
        "protocolVersion": "4",
        "keepalive": "60",
        "cleansession": true,
        "birthTopic": "",
        "birthQos": "0",
        "birthPayload": "",
        "birthMsg": {},
        "closeTopic": "",
        "closeQos": "0",
        "closePayload": "",
        "closeMsg": {},
        "willTopic": "",
        "willQos": "0",
        "willPayload": "",
        "willMsg": {},
        "sessionExpiry": ""
    },
    {
        "id": "a6c9238f876481dc",
        "type": "ui_group",
        "name": "카메라",
        "tab": "e523318c16bacab8",
        "order": 1,
        "disp": true,
        "width": "6",
        "collapse": false,
        "className": ""
    },
    {
        "id": "e523318c16bacab8",
        "type": "ui_tab",
        "name": "캡스톤디자인",
        "icon": "dashboard",
        "order": 5,
        "disabled": false,
        "hidden": false
    }
]
```   
