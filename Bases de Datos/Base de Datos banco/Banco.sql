#Archivo de creaación de esquemas para al BD del banco del Proyecto 1

#Creo la base de datos del Banco

CREATE DATABASE Banco;

#Selecciono la base de Datos recién creado

USE Banco;

#-------------------------------------------------------------------------
#Creo Tablas para las Entidades, recordar, Entidades Primero, relaciones después, para Herencia, crear la entidad padre primero y los hijos despues
#NOTA: VER TEMA CON LAS FECHAS, NO SE QUE TIPO DEBEN SER

CREATE TABLE Ciudad(

	cod_postal INT NOT NULL,
	nombre VARCHAR(45),

	CONSTRAINT pk_Ciudad
	PRIMARY KEY (cod_postal)

)Engine = InnoDB;

CREATE TABLE Sucursal(

	nro_suc numeric(3,0) NOT NULL,
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

	legajo numeric(4,0) NOT NULL,
	apellido VARCHAR(45),
	nombre VARCHAR(45),
	tipo_doc VARCHAR(45),
	nro_doc numeric(8,0),
	direccion VARCHAR(45),
	telefono VARCHAR(45),
	cargo VARCHAR(45),
	password VARCHAR(32),
	nro_suc numeric(3,0),

	CONSTRAINT pk_Emp
	PRIMARY KEY (legajo),

	CONSTRAINT FK_suc
	FOREIGN KEY (nro_suc) REFERENCES Sucursal (nro_suc)

)Engine = InnoDB;


CREATE TABLE Cliente(

	nro_cliente numeric(5,0) NOT NULL,
	apellido VARCHAR(45),
	nombre VARCHAR(45),
	tipo_doc VARCHAR(45),
	direccion VARCHAR(45),
	telefono VARCHAR(45),
	nro_doc numeric(8,0),
	fecha_nac VARCHAR(45),

	CONSTRAINT pk_nro_cliente
	PRIMARY KEY (nro_cliente),

)Engine = InnoDB;



CREATE TABLE Plazo_Fijo(

	nro_plazo numeric(8,0) NOT NULL,
	capital float(2),
	fecha_inicio VARCHAR(45),
	fecha_fin VARCHAR(45),
	tasa_interes float(2),
	interes float(2),
	nro_suc INT NOT NULL,

	CONSTRAINT pk_plazo
	PRIMARY KEY (nro_plazo), 

	CONSTRAINT FK_suc
	FOREIGN KEY (nro_suc) REFERENCES Sucursal (nro_suc)

)Engine = InnoDB;

CREATE TABLE Tasa_Plazo_Fijo(

	periodo numeric(3,0) NOT NULL,
	monto_inf float(2) NOT NULL,
	monto_sup float(2) NOT NULL,
	tasa float(2)

	CONSTRAINT pk_Tasa_Plazo
	PRIMARY KEY (periodo,monto_sup,monto_inf) 

)Engine = InnoDB;

CREATE TABLE Plazo_Cliente(

	nro_plazo numeric(8,0) NOT NULL,
	nro_cliente numeric(5,0) NOT NULL,

	CONSTRAINT FK_plazo
	FOREIGN KEY (nro_plazo) REFERENCES	Plazo_Fijo (nro_plazo),

	CONSTRAINT FK_cliente
	FOREIGN KEY (nro_cliente) REFERENCES Cliente (nro_cliente)

)Engine = InnoDB

CREATE TABLE Prestamo(

	nro_prestamo numeric(8,0) NOT NULL,
	fecha VARCHAR(45),
	cant_meses numeric(2,0),
	monto float(2),
	tasa_interes float(2),
	interes float(2),
	valor_cuota float(2),
	legajo numeric(4,0) NOT NULL,
	nro_cliente numeric(5,0) NOT NULL,

	CONSTRAINT pk_nro_prestamo
	PRIMARY KEY (nro_prestamo),

	CONSTRAINT FK_nro_cliente_prestamo
	FOREIGN KEY (nro_cliente) REFERENCES Cliente (nro_cliente),

	CONSTRAINT FK_nro_legajo_prestamo
	FOREIGN Key (legajo) REFERENCES Empleado (legajo)

)Engine = InnoDB;