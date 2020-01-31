---
title: LoRa-server setting
date: 2019-07-23 15:36:09
tags:
---


<https://www.loraserver.io>

## **종속성 설치**
```
$ sudo apt-get install postgresql
$ sudo apt-get install mosquitto
$ sudo apt-get install redis-server
```

## **데이터베이스 세팅**
```sql
-- create the loraserver_as user
create role loraserver_as with login password 'dbpassword';

-- create the loraserver_as database
create database loraserver_as with owner loraserver_as;

-- create the loraserver_ns user with password 'dbpassword'
create role loraserver_ns with login password 'dbpassword';

-- create the loraserver_ns database
create database loraserver_ns with owner loraserver_ns;

-- enable the trigram and hstore extensions
\c loraserver_as
create extension pg_trgm;
create extension hstore;

-- exit the prompt
\q
```

## **로라서버 데비안 저장소 등록**
$ sudo echo "deb https://artifacts.loraserver.io/packages/3.x/deb stable main" | sudo tee /etc/apt/sources.list.d/loraserver.list
$ sudo apt-get update

## **로라서버 관리 프로그램 설치**
$ sudo apt-get install loaserver
$ sudo apt-get install lora-app-server
$ sudo apt-get install lora-gateway-bridge

## **loraserver.toml 수정**
```toml
postgresql.dsn="postgres://loraserver_ns:password@localhost/loraserver_ns?sslmode=disable"
postgresql.automigrate=true
network_server.net_id=000000
network_server.band.name="KR_920_923"
metrics.timezone="Asia/Seoul"
```

## **lora-app-server.toml 수정**
```toml
postgresql.dsn="postgres://loraserver_as:password@localhost/loraserver_as?sslmode=disable"
jwt_secret=[openssl rand -base64 32]
```

