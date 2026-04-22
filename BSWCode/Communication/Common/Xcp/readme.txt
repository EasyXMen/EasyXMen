存放代码。
v1.1-2017.9.5
v1.2-2017.9.6(完善SET_REQUEST命令，解决CANAPE连接时Upload时报错)
v1.3-2017.9.8(解决了1.用CANAPE没法多次测量；2.在用单Eventchannel发送大数据时掉帧(目前在用短周期发送大数据时仍会出现OVERLOAD))
v1.4-2017.9.12(完善了了静态DAQ配置下，各种PID类型的DAQ的保存以及上电自动上传)
v1.5-2017.10.9(Xcp_DAQReinit()函数中局部变量定义提前；SET_REQUEST命令在保存DAQ后不做disconnect，由上位机去操作)
v1.6-2017.10.10(添加Build_checksum功能和Get_Slaveid功能)
v1.7-2017.10.16(完善DAQ上传时Overload情况处理)
v1.8-2018.1.25(1.规范版本检查 2.数据结构按照4.2.2进行了一定修改，增加pb配置 3.det上报修正)