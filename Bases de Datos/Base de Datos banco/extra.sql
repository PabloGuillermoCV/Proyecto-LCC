INSERT INTO Empleado (apellido,nombre,tipo_doc,nro_doc,direccion,telefono,cargo,password,nro_suc) 
			VALUES ("Ceballos","Pablo","dni",12345678,"CalleEmp4","456267","Cargo4",md5("pablo"),1);
INSERT INTO Tarjeta (PIN,CVT,fecha_venc,nro_cliente,nro_ca) 
			VALUES (md5("1234"), md5("5678"), STR_TO_DATE("08-08-2022, "%d-%m-%Y),1,12345678);
			