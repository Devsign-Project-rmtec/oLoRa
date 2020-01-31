---
title: LoRa 장비를 LoRaWAN에 연결
date: 2019-07-23 16:18:38
tags:
---

# **LoRa 장비를 LoRaWAN에 연결**
AT 명령어를 이용하여 장비를 설정하고 연결한다.
Line Ending은 <CR><LF>를 사용한다.

## **버전 확인**
`at+version`


## **연결 모드 설정**
0: OTTA, 1: ABP
`at+set_config=lora:join_mode:0`


## **LoRa 클래스 설정**
0: Class A, 1: Class B, 2: Class C
`at+set_config=lora:class:0`


## **주파수/지역 설정**
한국: KR920
`at+set_config=lora:region:KR920`


## **장비 EUI 설정**
`at+set_config=lora:dev_eui:(8byte hex eui)`


## **APP EUI 설정**
OTTA 모드에서는 이용하지 않으며, 형식에 맞게 아무 값이나 사용
`at+set_config=lora:app_eui:(8byte hex eui)`


## **APP KEY 설정**
LoRa Server의 Application에서 생성한 App key를 입력한다.
`at+set_config=lora:app_key:(16 byte hex key)`


## **연결 시작**
연결에 성공하면 LoRa Server에서 확인할 수 있다.
`at+join`


## **메시지 보내기**
메시지 전송에 성공하면 LoRa Server에서 확인할 수 있다.
`at+send=lora:2:1234567890`


******


{% asset_img IMG_6311.jpg %}
{% asset_img image0.png %}