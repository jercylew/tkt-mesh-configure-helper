# TKTMeshConfigureHelper

#### 介绍
TKTMesh的配置软件

#### 软件架构
软件架构说明
TKTMesh配置UI --> TKTMeshAgent --> MEsh Gateway ->BLE Node1, Node 2, ...., Node N
配置助手UI通过TCP Socket与Agent连接，可以向Agent发送命令和读取数据，Agent运行于工控机Linux环境中，通过串口RS232与Gateway连接
Gateway负责采集蓝牙节点数据，和发送命令至蓝牙节点
