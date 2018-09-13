INSERT INTO Ciudad (cod_postal,nombre) VALUES (8001,"Ciudad1");
INSERT INTO Ciudad (cod_postal,nombre) VALUES (8002,"Ciudad2");

INSERT INTO Sucursal (nombre,direccion,telefono,horario,cod_postal) VALUES ("Sucursal1","CalleSuc1","111111","24",8001);
INSERT INTO Sucursal (nombre,direccion,telefono,horario,cod_postal) VALUES ("Sucursal2","CalleSuc2","222222","24",8001);

INSERT INTO Empleado (apellido,nombre,tipo_doc,nro_doc,direccion,telefono,cargo,password,nro_suc) 
			VALUES ("AE1","NE1","dni",84532017,"CalleEmp1","147252","Cargo1",md5("password1"),1);
INSERT INTO Empleado (apellido,nombre,tipo_doc,nro_doc,direccion,telefono,cargo,password,nro_suc) 
			VALUES ("AE2","NE2","dni",13245678,"CalleEmp2","786521","Cargo2",md5("password2"),1);
INSERT INTO Empleado (apellido,nombre,tipo_doc,nro_doc,direccion,telefono,cargo,password,nro_suc) 
			VALUES ("AE3","NE3","dni",20314565,"CalleEmp3","456217","Cargo3",md5("password3"),1);

INSERT INTO Cliente (apellido,nombre,tipo_doc,nro_doc,direccion,telefono,fecha_nac) 
			VALUES ("AC1","NC1","dni",01356489,"CalleCli1","453210",STR_TO_DATE("05-04-2000","%d-%m-%Y"));
INSERT INTO Cliente (apellido,nombre,tipo_doc,nro_doc,direccion,telefono,fecha_nac) 
			VALUES ("AC2","NC2","dni",13645820,"CalleCli2","302124",STR_TO_DATE("15-03-1998","%d-%m-%Y"));
INSERT INTO Cliente (apellido,nombre,tipo_doc,nro_doc,direccion,telefono,fecha_nac) 
			VALUES ("AC3","NC3","dni",30124872,"CalleCli3","134251",STR_TO_DATE("07-04-2001","%d-%m-%Y"));
INSERT INTO Cliente (apellido,nombre,tipo_doc,nro_doc,direccion,telefono,fecha_nac) 
			VALUES ("AC4","NC4","dni",65487952,"CalleCli4","134247",STR_TO_DATE("20-10-2000","%d-%m-%Y"));

INSERT INTO Plazo_Fijo (capital,fecha_inicio,fecha_fin,tasa_interes,interes,nro_suc) 
			VALUES (200.02,STR_TO_DATE("10-09-2018","%d-%m-%Y"),STR_TO_DATE("05-11-2018","%d-%m-%Y"),10.02,32.45,1);
INSERT INTO Plazo_Fijo (capital,fecha_inicio,fecha_fin,tasa_interes,interes,nro_suc) 
			VALUES (250.85,STR_TO_DATE("06-08-2018","%d-%m-%Y"),STR_TO_DATE("22-12-2018","%d-%m-%Y"),20.78,25.12,1);

INSERT INTO Tasa_Plazo_Fijo (periodo,monto_inf,monto_sup,tasa) VALUES (245,423.44,782.12,45.11);
INSERT INTO Tasa_Plazo_Fijo (periodo,monto_inf,monto_sup,tasa) VALUES (245,256.66,532.57,87.45);

INSERT INTO Plazo_Cliente (nro_plazo,nro_cliente) VALUES (1,1);
INSERT INTO Plazo_Cliente (nro_plazo,nro_cliente) VALUES (2,2);

INSERT INTO Prestamo (fecha,cant_meses,monto,tasa_interes,interes,valor_cuota,legajo,nro_cliente) 
			VALUES (STR_TO_DATE("15-09-2018","%d-%m-%Y"),4,120.55,12.48,98.66,2.35,1,1);
INSERT INTO Prestamo (fecha,cant_meses,monto,tasa_interes,interes,valor_cuota,legajo,nro_cliente) 
			VALUES (STR_TO_DATE("14-11-2018","%d-%m-%Y"),5,782.01,98.02,46.52,3.12,2,2);
INSERT INTO Prestamo (fecha,cant_meses,monto,tasa_interes,interes,valor_cuota,legajo,nro_cliente) 
			VALUES (STR_TO_DATE("23-12-2018","%d-%m-%Y"),3,352.01,45.58,45.22,2.98,1,3);

INSERT INTO Pago (nro_prestamo,nro_pago,fecha_venc,fecha_pago) 
			VALUES (1,01,STR_TO_DATE("15-01-2019","%d-%m-%Y"),STR_TO_DATE("02-01-2019","%d-%m-%Y"));
INSERT INTO Pago (nro_prestamo,nro_pago,fecha_venc,fecha_pago) 
			VALUES (2,03,STR_TO_DATE("14-03-2019","%d-%m-%Y"),NULL);
INSERT INTO Pago (nro_prestamo,nro_pago,fecha_venc,fecha_pago) 
			VALUES (3,03,STR_TO_DATE("23-04-2019","%d-%m-%Y"),STR_TO_DATE("20-04-2019","%d-%m-%Y"));

INSERT INTO Tasa_Prestamo (periodo,monto_inf,monto_sup,tasa) VALUES (123,45.02,78.56,12.35);
INSERT INTO Tasa_Prestamo (periodo,monto_inf,monto_sup,tasa) VALUES (123,12.47,65.25,12.35);
INSERT INTO Tasa_Prestamo (periodo,monto_inf,monto_sup,tasa) VALUES (123,77.54,99.56,45.08);

INSERT INTO Caja_Ahorro (nro_ca,CBU,saldo) VALUES (12345678,1432147678961345,100.45);
INSERT INTO Caja_Ahorro (nro_ca,CBU,saldo) VALUES (23456789,6054870514705220,188.32);
INSERT INTO Caja_Ahorro (nro_ca,CBU,saldo) VALUES (34567891,2013450650457865,142.98);
INSERT INTO Caja_Ahorro (nro_ca,CBU,saldo) VALUES (45678912,4243567894123102,132.75);

INSERT INTO Cliente_ca (nro_cliente,nro_ca) VALUES (1,12345678);
INSERT INTO Cliente_ca (nro_cliente,nro_ca) VALUES (2,23456789);
INSERT INTO Cliente_ca (nro_cliente,nro_ca) VALUES (3,34567891);
INSERT INTO Cliente_ca (nro_cliente,nro_ca) VALUES (4,45678912);

INSERT INTO Tarjeta (PIN,CVT,fecha_venc,nro_cliente,nro_ca) 
			VALUES (md5("ajfkdlsiwuehcndmxk78945610234786"),md5("pdowlaksmcxbzjkjgvhjkds4561231562"),STR_TO_DATE("10-08-2022","%d-%m-%Y"),1,12345678);
INSERT INTO Tarjeta (PIN,CVT,fecha_venc,nro_cliente,nro_ca) 
			VALUES (md5("dhgfd67h14gfd56453d1bgfdbhgfd654"),md5("opuqiopehjklsdaf45602312348975310"),STR_TO_DATE("14-08-2022","%d-%m-%Y"),2,23456789);
INSERT INTO Tarjeta (PIN,CVT,fecha_venc,nro_cliente,nro_ca) 
			VALUES (md5("gfds67g8fdsgfdsgfds1231gr65ew7t1"),md5("efwqfdsa5403201234fdsaf4g65456405"),STR_TO_DATE("30-09-2022","%d-%m-%Y"),3,34567891);
INSERT INTO Tarjeta (PIN,CVT,fecha_venc,nro_cliente,nro_ca) 
			VALUES (md5("gerw60w103fd4s56741894530123sdg4"),md5("wytre97poiu41255360gkjhgkjhg7iuy9"),STR_TO_DATE("15-02-2023","%d-%m-%Y"),4,45678912);

INSERT INTO Caja () VALUES ();
INSERT INTO Caja () VALUES ();
INSERT INTO Caja () VALUES ();
INSERT INTO Caja () VALUES ();

INSERT INTO Ventanilla (cod_caja,nro_suc) VALUES (1,1);
INSERT INTO Ventanilla (cod_caja,nro_suc) VALUES (2,1);
INSERT INTO Ventanilla (cod_caja,nro_suc) VALUES (3,1);
INSERT INTO Ventanilla (cod_caja,nro_suc) VALUES (4,1);

INSERT INTO ATM (cod_caja,cod_postal,direccion) VALUES (1,8001,"DirATM1");
INSERT INTO ATM (cod_caja,cod_postal,direccion) VALUES (2,8001,"DirATM2");
INSERT INTO ATM (cod_caja,cod_postal,direccion) VALUES (3,8001,"DirATM3");
INSERT INTO ATM (cod_caja,cod_postal,direccion) VALUES (4,8001,"DirATM4");

INSERT INTO Transaccion (fecha,hora,monto) VALUES (STR_TO_DATE("06-12-2018","%d-%m-%Y"),HOUR("04"),45.20);
INSERT INTO Transaccion (fecha,hora,monto) VALUES (STR_TO_DATE("02-12-2018","%d-%m-%Y"),HOUR("11"),78.50);
INSERT INTO Transaccion (fecha,hora,monto) VALUES (STR_TO_DATE("12-12-2018","%d-%m-%Y"),HOUR("05"),65.32);
INSERT INTO Transaccion (fecha,hora,monto) VALUES (STR_TO_DATE("15-12-2018","%d-%m-%Y"),HOUR("07"),75.21);
INSERT INTO Transaccion (fecha,hora,monto) VALUES (STR_TO_DATE("17-12-2018","%d-%m-%Y"),HOUR("03"),45.12);
INSERT INTO Transaccion (fecha,hora,monto) VALUES (STR_TO_DATE("22-12-2018","%d-%m-%Y"),HOUR("04"),95.37);
INSERT INTO Transaccion (fecha,hora,monto) VALUES (STR_TO_DATE("28-12-2018","%d-%m-%Y"),HOUR("10"),82.14);
INSERT INTO Transaccion (fecha,hora,monto) VALUES (STR_TO_DATE("30-12-2018","%d-%m-%Y"),HOUR("01"),65.27);

INSERT INTO Debito (nro_trans,descripcion,nro_cliente,nro_ca) VALUES (1,"desc1",1,12345678);
INSERT INTO Debito (nro_trans,descripcion,nro_cliente,nro_ca) VALUES (2,"desc2",2,23456789);
INSERT INTO Debito (nro_trans,descripcion,nro_cliente,nro_ca) VALUES (3,"desc3",3,34567891);
INSERT INTO Debito (nro_trans,descripcion,nro_cliente,nro_ca) VALUES (4,"desc4",4,45678912);

INSERT INTO Transaccion_por_caja (nro_trans,cod_caja) VALUES (5,1);
INSERT INTO Transaccion_por_caja (nro_trans,cod_caja) VALUES (6,2);
INSERT INTO Transaccion_por_caja (nro_trans,cod_caja) VALUES (7,3);
INSERT INTO Transaccion_por_caja (nro_trans,cod_caja) VALUES (8,4);

INSERT INTO Deposito (nro_trans,nro_ca) VALUES (5,12345678);
INSERT INTO Deposito (nro_trans,nro_ca) VALUES (6,23456789);
INSERT INTO Deposito (nro_trans,nro_ca) VALUES (7,34567891);
INSERT INTO Deposito (nro_trans,nro_ca) VALUES (8,45678912);

INSERT INTO Extraccion (nro_trans,nro_cliente,nro_ca) VALUES (5,1,12345678);
INSERT INTO Extraccion (nro_trans,nro_cliente,nro_ca) VALUES (6,2,23456789);
INSERT INTO Extraccion (nro_trans,nro_cliente,nro_ca) VALUES (7,3,34567891);
INSERT INTO Extraccion (nro_trans,nro_cliente,nro_ca) VALUES (8,4,45678912);

INSERT INTO Transferencia (nro_trans,nro_cliente,origen,destino) VALUES (5,1,12345678,23456789);
INSERT INTO Transferencia (nro_trans,nro_cliente,origen,destino) VALUES (6,2,23456789,12345678);
INSERT INTO Transferencia (nro_trans,nro_cliente,origen,destino) VALUES (7,3,34567891,23456789);
INSERT INTO Transferencia (nro_trans,nro_cliente,origen,destino) VALUES (8,4,45678912,45678912);