#Archivo de creaación de esquemas para al BD del banco del Proyecto 1

#Creo la base de datos del Banco

CREATE DATABASE banco;

#Selecciono la base de Datos recién creado

USE banco;

#-------------------------------------------------------------------------

#Creo Tablas para las Entidades, recordar, Entidades Primero, relaciones después

#LAS CLAVES QUE HEREDAN SE CLASIFICAN COMO FORANEAS EN SU PROPIA TABLA?

CREATE TABLE Ciudad(

	cod_postal INT NOT NULL,
	nombre VARCHAR(45),

	CONSTRAINT pk_Ciudad
	PRIMARY KEY (cod_postal)

)Engine = InnoDB;

CREATE TABLE Sucursal(

	nro_suc numeric(3,0) UNSIGNED NOT NULL,
	nombre VARCHAR(45),
	direccion VARCHAR(45),
	telefono VARCHAR(45),
	horario VARCHAR(45),
	cod_postal INT NOT NULL,

	CONSTRAINT pk_suc
	PRIMARY KEY (nro_suc),

	CONSTRAINT FK_postal
	FOREIGN KEY (cod_postal) REFERENCES Ciudad (cod_postal)
		ON DELETE RESTRICT ON UPDATE CASCADE
	

)Engine = InnoDB;

CREATE TABLE Empleado(

	legajo numeric(4,0) UNSIGNED NOT NULL,
	apellido VARCHAR(45),
	nombre VARCHAR(45),
	tipo_doc VARCHAR(45),
	nro_doc numeric(8,0) UNSIGNED,
	direccion VARCHAR(45),
	telefono VARCHAR(45),
	cargo VARCHAR(45),
	password VARCHAR(32),
	nro_suc numeric(3,0) NOT NULL,

	CONSTRAINT pk_Emp
	PRIMARY KEY (legajo),

	CONSTRAINT FK_suc
	FOREIGN KEY (nro_suc) REFERENCES Sucursal (nro_suc)

)Engine = InnoDB;

CREATE TABLE Cliente(

	nro_cliente numeric(5,0) UNSIGNED NOT NULL,
	apellido VARCHAR(45),
	nombre VARCHAR(45),
	tipo_doc VARCHAR(45),
	direccion VARCHAR(45),
	telefono VARCHAR(45),
	nro_doc numeric(8,0) UNSIGNED,
	fecha_nac DATE,

	CONSTRAINT pk_nro_cliente
	PRIMARY KEY (nro_cliente),

)Engine = InnoDB;

CREATE TABLE Plazo_Fijo(

	nro_plazo numeric(8,0) UNSIGNED NOT NULL,
	capital float(2) UNSIGNED,
	fecha_inicio DATE,
	fecha_fin DATE,
	tasa_interes float(2) UNSIGNED,
	interes float(2) UNSIGNED,
	nro_suc INT UNSIGNED NOT NULL,

	CONSTRAINT pk_plazo
	PRIMARY KEY (nro_plazo), 

	CONSTRAINT FK_suc
	FOREIGN KEY (nro_suc) REFERENCES Sucursal (nro_suc)

)Engine = InnoDB;

CREATE TABLE Tasa_Plazo_Fijo(

	periodo numeric(3,0) UNSIGNED NOT NULL,
	monto_inf float(2) UNSIGNED NOT NULL,
	monto_sup float(2) UNSIGNED NOT NULL,
	tasa float(2),

	CONSTRAINT pk_Tasa_Plazo
	PRIMARY KEY (periodo,monto_sup,monto_inf) 

)Engine = InnoDB;

CREATE TABLE Plazo_Cliente(

	nro_plazo numeric(8,0) UNSIGNED NOT NULL,
	nro_cliente numeric(5,0) UNSIGNED NOT NULL,
	
	CONSTRAINT pk_plazo_cliente
	PRIMARY KEY (nro_plazo,nro_cliente)

	CONSTRAINT FK_plazo
	FOREIGN KEY (nro_plazo) REFERENCES	Plazo_Fijo (nro_plazo),

	CONSTRAINT FK_cliente
	FOREIGN KEY (nro_cliente) REFERENCES Cliente (nro_cliente)

)Engine = InnoDB

CREATE TABLE Prestamo(

	nro_prestamo numeric(8,0) UNSIGNED NOT NULL,
	fecha DATE,
	cant_meses numeric(2,0) UNSIGNED,
	monto float(2) UNSIGNED,
	tasa_interes float(2) UNSIGNED,
	interes float(2) UNSIGNED,
	valor_cuota float(2) UNSIGNED,
	legajo numeric(4,0) NOT NULL,
	nro_cliente numeric(5,0) NOT NULL,

	CONSTRAINT pk_nro_prestamo
	PRIMARY KEY (nro_prestamo),

	CONSTRAINT FK_nro_cliente_prestamo
	FOREIGN KEY (nro_cliente) REFERENCES Cliente (nro_cliente),

	CONSTRAINT FK_nro_legajo_prestamo
	FOREIGN Key (legajo) REFERENCES Empleado (legajo)

)Engine = InnoDB;

CREATE TABLE Pago{

	nro_prestamo numeric (8,0) UNSIGNED NOT NULL,
	nro_pago numeric (2,0) UNSIGNED NOT NULL,
	fecha_venc DATE,
	fecha_pago DATE,
	
	CONSTRAINT pk_nro_pago
	PRIMARY KEY (nro_pago),
	
	CONSTRAINT po_nro_prestamo
	PRIMARY KEY (nro_prestamo),
	
	CONSTRAINT FK_nro_prestamo
	FOREIGN KEY (nro_prestamo) REFERENCES Prestamo (nro_prestamo)
	
}Engine = InnoDB;

CREATE TABLE Tasa_Prestamo{

	periodo numeric (3,0) UNSIGNED NOT NULL,
	monto_inf float (2) UNSIGNED,
	monto_sup float (2) UNSIGNED,
	tasa float (2) UNSIGNED,
	
	CONSTRAINT pk_tasa_prestamo
	PRIMARY KEY (periodo,monto_inf,monto_sup)
	
}Engine = InnoDB;

CREATE TABLE Caja_Ahorro{

	nro_ca numeric (8,0) UNSIGNED NOT NULL,
	CBU numeric (18,0) UNSIGNED NOT NULL,
	saldo float (2) UNSIGNED,
	
	CONSTRAINT pk_nro_ca
	PRIMARY KEY (nro_ca)
	
}Engine = InnoDB;

CREATE TABLE Cliente_CA{

	nro_cliente numeric(5,0) UNSIGNED NOT NULL,
	nro_ca numeric (8,0) UNSIGNED NOT NULL,
	
	CONSTRAINT pk_cliente_ca
	PRIMARY KEY (nro_cliente,nro_ca),
	
	CONSTRAINT FK_nro_cliente
	FOREIGN KEY (nro_cliente) REFERENCES Cliente (nro_cliente),
	
	CONSTRAINT FK_nro_ca
	FOREIGN KEY (nro_ca) REFERENCES Caja_Ahorro (nro_ca)
	
}Engine = InnoDB;

CREATE TABLE Tarjeta{

	nro_tarjeta numeric(16,0) UNSIGNED NOT NULL,
	PIN VARCHAR(32),
	CVT VARCHAR(32),
	fecha_venc DATE,
	nro_cliente numeric(5,0) UNSIGNED NOT NULL,
	nro_ca numeric (8,0) UNSIGNED NOT NULL,
	
	CONSTRAINT pk_tarjeta
	PRIMARY KEY (nro_tarjeta),
	
	CONSTRAINT FK_nro_cliente
	FOREIGN KEY (nro_cliente) REFERENCES Cliente (nro_cliente),
	
	CONSTRAINT FK_nro_ca
	FOREIGN KEY (nro_ca) REFERENCES Caja_Ahorro (nro_ca)
	
}Engine = InnoDB;

CREATE TABLE Caja{

	cod_caja numeric(5,0) UNSIGNED,
	
	CONSTRAINT pk_caja
	PRIMARY KEY (cod_caja)
	
}Engine = InnoDB;

CREATE TABLE Ventanilla{

	cod_caja numeric(5,0) UNSIGNED NOT NULL,
	nro_suc numeric(3,0) UNSIGNED NOT NULL,
	
	CONSTRAINT pk_ventanilla
	PRIMARY KEY (cod_caja),
	
	CONSTRAINT FK_cod_caja
	FOREIGN KEY (cod_caja) REFERENCES Caja (cod_caja),
	
	CONSTRAINT FK_nro_suc
	FOREIGN KEY (nro_suc) REFERENCES Sucursal (nro_suc)
	
}Engine = InnoDB;

CREATE TABLE ATM{

	cod_caja numeric(5,0) UNSIGNED NOT NULL,
	cod_postal INT UNSIGNED NOT NULL,
	direccion VARCHAR(45),
	
	CONSTRAINT pk_atm
	PRIMARY KEY (cod_caja),
	
	CONSTRAINT FK_cod_caja
	FOREIGN KEY (cod_caja) REFERENCES Caja (cod_caja),
	
	CONSTRAINT FK_cod_postal
	FOREIGN KEY (cod_postal) REFERENCES Ciudad (cod_postal)
	
}Engine = InnoDB;

CREATE TABLE Transaccion{

	nro_trans numeric(10,0) UNSIGNED NOT NULL,
	fecha DATE,
	hora VARCHAR(45),
	monto float (2) UNSIGNED,
	
	CONSTRAINT pk_transaccion
	PRIMARY KEY (nro_trans)
	
}Engine = InnoDB;

CREATE TABLE Debito{

	nro_trans numeric(10,0) UNSIGNED NOT NULL,
	descripcion VARCHAR(45),
	nro_cliente numeric(5,0) UNSIGNED NOT NULL,
	nro_ca numeric (8,0) UNSIGNED NOT NULL,
	
	CONSTRAINT pk_debito
	PRIMARY KEY (nro_trans),
	
	CONSTRAINT FK_nro_trans
	FOREIGN KEY (nro_trans) REFERENCES Transaccion (nro_trans),
	
	CONSTRAINT FK_nro_cliente
	FOREIGN KEY (nro_cliente) REFERENCES Cliente_CA (nro_cliente),
	
	CONSTRAINT FK_nro_ca
	FOREIGN KEY (nro_ca) REFERENCES Cliente_CA (nro_ca)
	
}Engine = InnoDB;

CREATE TABLE Transaccion_por_caja{

	nro_trans numeric(10,0) UNSIGNED NOT NULL,
	cod_caja numeric(5,0) UNSIGNED NOT NULL,
	
	CONSTRAINT pk_transaccion_por_caja
	PRIMARY KEY (nro_trans),
	
	CONSTRAINT FK_nro_trans
	FOREIGN KEY (nro_trans) REFERENCES Transaccion (nro_trans),
	
	CONSTRAINT FK_cod_caja
	FOREIGN KEY (cod_caja) REFERENCES Caja (cod_caja)
	
}Engine = InnoDB;

CREATE TABLE Deposito{

	nro_trans numeric(10,0) UNSIGNED NOT NULL,
	nro_ca numeric (8,0) UNSIGNED NOT NULL,
	
	CONSTRAINT pk_deposito
	PRIMARY KEY (nro_trans),
	
	CONSTRAINT FK_nro_trans
	FOREIGN KEY (nro_trans) REFERENCES Transaccion_por_caja (nro_trans),
	
	CONSTRAINT FK_cod_caja
	FOREIGN KEY (nro_ca) REFERENCES Caja_Ahorro (nro_ca)
	
}Engine = InnoDB;

CREATE TABLE Extraccion{

	nro_trans numeric(10,0) UNSIGNED NOT NULL,
	nro_cliente numeric(5,0) UNSIGNED NOT NULL,
	nro_ca numeric (8,0) UNSIGNED NOT NULL,
	
	CONSTRAINT pk_extraccion
	PRIMARY KEY (nro_trans),
	
	CONSTRAINT FK_nro_trans
	FOREIGN KEY (nro_trans) REFERENCES Transaccion_por_caja (nro_trans),
	
	CONSTRAINT FK_nro_cliente
	FOREIGN KEY (nro_cliente) REFERENCES Cliente_CA (nro_cliente),
	
	CONSTRAINT FK_nro_ca
	FOREIGN KEY (nro_ca) REFERENCES Cliente_CA (nro_ca)
	
}Engine = InnoDB;

CREATE TABLE Transferencia{

	nro_trans numeric(10,0) UNSIGNED NOT NULL,
	nro_cliente numeric(5,0) UNSIGNED NOT NULL,
	origen numeric (8,0) UNSIGNED NOT NULL,
	destino numeric (8,0) UNSIGNED NOT NULL,
	
	CONSTRAINT pk_transferencia
	PRIMARY KEY (nro_trans),
	
	CONSTRAINT FK_nro_trans
	FOREIGN KEY (nro_trans) REFERENCES Transaccion_por_caja (nro_trans),
	
	CONSTRAINT FK_nro_cliente
	FOREIGN KEY (nro_cliente) REFERENCES Cliente_CA (nro_cliente),
	
	CONSTRAINT FK_origen
	FOREIGN KEY (origen) REFERENCES Cliente_CA (nro_ca),
	
	CONSTRAINT FK_destino
	FOREIGN KEY (destino) REFERENCES Caja_Ahorro (nro_ca)
	
}Engine = InnoDB;

#-------------------------------------------------------------------------

#Creacion de vistas

	CREATE VIEW trans_caja_ahorro AS
	SELECT
	FROM
	WHERE ;

	#SELECT 'debito' AS tipo

#-------------------------------------------------------------------------

#EL VIEW ESTA SIN TERMINAR, DUDO DE COMO HACERLO //consulta super compleja, hacer tp3, hacerla por partes, 4 vistas distintas, luego agrupar

#Creacion de usuarios y otorgamiento de privilegios.

	DROP USER ''@'localhost';

#Usuario admin con acceso total sobre todas las tablas y la posibilidad de crear usuarios y otorgar privilegios

	CREATE USER 'admin'@'localhost' IDENTIFIED BY 'admin';
	
	GRANT ALL PRIVILEGES ON banco.* TO 'admin'@'localhost' WITH GRANT OPTION;

#Usuario empleado

	CREATE USER 'empleado'@'%' IDENTIFIED BY 'empleado';
	
	GRANT SELECT ON banco.Empleado, banco.Sucursal, banco.Tasa_Plazo_Fijo, banco.Tasa_Prestamo TO 'empleado'@'%';
	
	GRANT SELECT ON banco.Prestamo, banco.Plazo_Fijo, banco.Plazo_Cliente, banco.Caja_Ahorro, banco.Tarjeta TO 'empleado'@'%';
	
	GRANT INSERT ON banco.Prestamo, banco.Plazo_Fijo, banco.Plazo_Cliente, banco.Caja_Ahorro, banco.Tarjeta TO 'empleado'@'%';
	
	GRANT SELECT ON banco.Cliente_CA, banco.Cliente, banco.Pago TO 'empleado'@'%';
	
	GRANT INSERT ON banco.Cliente_CA, banco.Cliente, banco.Pago TO 'empleado'@'%';
	
	GRANT UPDATE ON banco.Cliente_CA, banco.Cliente, banco.Pago TO 'empleado'@'%';

#Usuario atm

	CREATE USER 'atm'@'%' IDENTIFIED BY 'atm';
	
	GRANT SELECT ON banco.trans_caja_ahorro TO 'atm'@'%';

#-------------------------------------------------------------------------