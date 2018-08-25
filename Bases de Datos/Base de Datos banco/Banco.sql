#Archivo de creaación de esquemas para al BD del banco del Proyecto 1

#Creo la base de datos del Banco

CREATE DATABASE Banco;

#Selecciono la base de Datos recién creado

USE Banco;

#-------------------------------------------------------------------------
#Creo Tablas para las Entidades, recordar, Entidades Primero, relaciones después, para Herencia, crear la entidad padre primero y los hijos despues

CREATE TABLE Persona(

	Nombre VARCHAR(45) NOT NULL,
	Tipo_Documento VARCHAR(10) NOT NULL,
	Apellido VARCHAR(45) NOT NULL,
	Telefono INT,
	Nro_Documento INT NOT NULL,

	CONSTRAINT pk_Documento
	PRIMARY KEY (Nro_Documento)

)Engine = InnoDB;

CREATE TABLE Empleado(

	cargo VARCHAR(45) NOT NULL,
	password VARCHAR(45) NOT NULL,
	Nro_Legajo INT NOT NULL,
	Documento_Empleado INT NOT NULL,


	CONSTRAINT pk_Legajo
	PRIMARY KEY (Nro_Legajo),

	CONSTRAINT FK_Docu
	FOREIGN KEY (Documento_Empleado) REFERENCES Persona (Nro_Documento)
		ON DELETE RESTRICT ON UPDATE CASCADE,

)Engine = InnoDB