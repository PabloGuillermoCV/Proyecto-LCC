#Archivo de creaación de esquemas para al BD del banco del Proyecto 1

#Creo la base de datos del Banco

CREATE DATABASE banco;

#Selecciono la base de Datos recién creado

USE banco;

#-------------------------------------------------------------------------

#Creo Tablas para las Entidades, recordar, Entidades Primero, relaciones después

CREATE TABLE Ciudad(

	cod_postal INT UNSIGNED NOT NULL,
	nombre VARCHAR(45) NOT NULL,

	CONSTRAINT pk_Ciudad
	PRIMARY KEY (cod_postal)

)Engine = InnoDB;

CREATE TABLE Sucursal(

	nro_suc SMALLINT UNSIGNED AUTO_INCREMENT NOT NULL,
	nombre VARCHAR(45) NOT NULL,
	direccion VARCHAR(45) NOT NULL,
	telefono VARCHAR(45) NOT NULL,
	horario VARCHAR(45) NOT NULL,
	cod_postal INT UNSIGNED NOT NULL,

	CONSTRAINT pk_suc
	PRIMARY KEY (nro_suc),

	CONSTRAINT FK_postal
	FOREIGN KEY (cod_postal) REFERENCES Ciudad (cod_postal)
		ON DELETE RESTRICT ON UPDATE CASCADE
	

)Engine = InnoDB;

CREATE TABLE Empleado(

	legajo SMALLINT UNSIGNED AUTO_INCREMENT NOT NULL,
	apellido VARCHAR(45) NOT NULL,
	nombre VARCHAR(45) NOT NULL,
	tipo_doc VARCHAR(20) NOT NULL,
	nro_doc INT UNSIGNED NOT NULL,
	direccion VARCHAR(45) NOT NULL,
	telefono VARCHAR(45) NOT NULL,
	cargo VARCHAR(45) NOT NULL,
	password VARCHAR(32) NOT NULL,
	nro_suc SMALLINT UNSIGNED NOT NULL,

	CONSTRAINT pk_Emp
	PRIMARY KEY (legajo),

	CONSTRAINT FK_suc_E
	FOREIGN KEY (nro_suc) REFERENCES Sucursal (nro_suc)

)Engine = InnoDB;

CREATE TABLE Cliente(

	nro_cliente SMALLINT UNSIGNED AUTO_INCREMENT NOT NULL,
	apellido VARCHAR(45) NOT NULL,
	nombre VARCHAR(45) NOT NULL,
	tipo_doc VARCHAR(20) NOT NULL,
	nro_doc INT UNSIGNED NOT NULL,
	direccion VARCHAR(45) NOT NULL,
	telefono VARCHAR(45) NOT NULL,
	fecha_nac DATE NOT NULL,

	CONSTRAINT pk_nro_cliente
	PRIMARY KEY (nro_cliente)

)Engine = InnoDB;

CREATE TABLE Plazo_Fijo(

	nro_plazo INT UNSIGNED AUTO_INCREMENT NOT NULL,
	capital DECIMAL(16,2) UNSIGNED NOT NULL,
	fecha_inicio DATE NOT NULL,
	fecha_fin DATE NOT NULL,
	tasa_interes DECIMAL(4,2) UNSIGNED NOT NULL,
	interes DECIMAL(16,2) UNSIGNED NOT NULL,
	nro_suc SMALLINT UNSIGNED NOT NULL,

	CONSTRAINT pk_plazo
	PRIMARY KEY (nro_plazo), 

	CONSTRAINT FK_suc_PF
	FOREIGN KEY (nro_suc) REFERENCES Sucursal (nro_suc)

)Engine = InnoDB;

CREATE TABLE Tasa_Plazo_Fijo(

	periodo INT UNSIGNED NOT NULL,
	monto_inf DECIMAL(16,2) UNSIGNED NOT NULL,
	monto_sup DECIMAL(16,2) UNSIGNED NOT NULL,
	tasa DECIMAL(4,2) UNSIGNED NOT NULL,

	CONSTRAINT pk_Tasa_Plazo
	PRIMARY KEY (periodo,monto_sup,monto_inf) 

)Engine = InnoDB;

CREATE TABLE Plazo_Cliente(

	nro_plazo INT UNSIGNED NOT NULL,
	nro_cliente SMALLINT UNSIGNED NOT NULL,
	
	CONSTRAINT pk_plazo_cliente
	PRIMARY KEY (nro_plazo,nro_cliente),

	CONSTRAINT FK_plazo
	FOREIGN KEY (nro_plazo) REFERENCES	Plazo_Fijo (nro_plazo),

	CONSTRAINT FK_cliente
	FOREIGN KEY (nro_cliente) REFERENCES Cliente (nro_cliente)

)Engine = InnoDB;

CREATE TABLE Prestamo(

	nro_prestamo INT UNSIGNED AUTO_INCREMENT NOT NULL,
	fecha DATE NOT NULL,
	cant_meses SMALLINT UNSIGNED NOT NULL,
	monto DECIMAL(10,2) UNSIGNED NOT NULL,
	tasa_interes DECIMAL(4,2) UNSIGNED NOT NULL,
	interes DECIMAL(9,2) UNSIGNED NOT NULL,
	valor_cuota DECIMAL(9,2) UNSIGNED NOT NULL,
	legajo SMALLINT UNSIGNED NOT NULL,
	nro_cliente SMALLINT UNSIGNED NOT NULL,

	CONSTRAINT pk_nro_prestamo
	PRIMARY KEY (nro_prestamo),

	CONSTRAINT FK_nro_cliente_prestamo
	FOREIGN KEY (nro_cliente) REFERENCES Cliente (nro_cliente),

	CONSTRAINT FK_nro_legajo_prestamo
	FOREIGN Key (legajo) REFERENCES Empleado (legajo)

)Engine = InnoDB;

CREATE TABLE Pago(

	nro_prestamo INT UNSIGNED NOT NULL,
	nro_pago INT UNSIGNED NOT NULL,
	fecha_venc DATE NOT NULL,
	fecha_pago DATE,
	
	CONSTRAINT pk_pago
	PRIMARY KEY (nro_pago, nro_prestamo),
	
	CONSTRAINT FK_nro_prestamo
	FOREIGN KEY (nro_prestamo) REFERENCES Prestamo (nro_prestamo)
	
)Engine = InnoDB;

CREATE TABLE Tasa_Prestamo(

	periodo INT UNSIGNED NOT NULL,
	monto_inf DECIMAL(10,2) UNSIGNED,
	monto_sup DECIMAL(10,2) UNSIGNED,
	tasa DECIMAL(4,2) UNSIGNED NOT NULL,
	
	CONSTRAINT pk_tasa_prestamo
	PRIMARY KEY (periodo,monto_inf,monto_sup)
	
)Engine = InnoDB;

CREATE TABLE Caja_Ahorro(

	nro_ca INT UNSIGNED AUTO_INCREMENT NOT NULL,
	CBU BIGINT UNSIGNED NOT NULL,
	saldo DECIMAL(16,2) UNSIGNED NOT NULL,
	
	CONSTRAINT pk_nro_ca
	PRIMARY KEY (nro_ca)
	
)Engine = InnoDB;

CREATE TABLE Cliente_ca(

	nro_cliente SMALLINT UNSIGNED NOT NULL,
	nro_ca INT UNSIGNED NOT NULL,

	
	CONSTRAINT pk_cliente_ca
	PRIMARY KEY (nro_cliente,nro_ca),
	
	CONSTRAINT FK_nro_cliente_CCA
	FOREIGN KEY (nro_cliente) REFERENCES Cliente (nro_cliente),
	
	CONSTRAINT FK_nro_ca_CCA
	FOREIGN KEY (nro_ca) REFERENCES Caja_Ahorro (nro_ca)
	
)Engine = InnoDB;

CREATE TABLE Tarjeta(

	nro_tarjeta BIGINT UNSIGNED AUTO_INCREMENT NOT NULL,
	PIN VARCHAR(32) NOT NULL,
	CVT VARCHAR(32) NOT NULL,
	fecha_venc DATE NOT NULL,
	nro_cliente SMALLINT UNSIGNED NOT NULL,
	nro_ca INT UNSIGNED NOT NULL,
	
	CONSTRAINT pk_tarjeta
	PRIMARY KEY (nro_tarjeta),
	
	CONSTRAINT FK_nro_cliente_T
	FOREIGN KEY (nro_cliente,nro_ca) REFERENCES Cliente_ca (nro_cliente,nro_ca)
	
)Engine = InnoDB;

CREATE TABLE Caja(

	cod_caja SMALLINT UNSIGNED AUTO_INCREMENT,
	
	CONSTRAINT pk_caja
	PRIMARY KEY (cod_caja)
	
)Engine = InnoDB;

CREATE TABLE Ventanilla(

	cod_caja SMALLINT UNSIGNED NOT NULL,
	nro_suc SMALLINT UNSIGNED NOT NULL,
	
	CONSTRAINT pk_ventanilla
	PRIMARY KEY (cod_caja),
	
	CONSTRAINT FK_cod_caja_V
	FOREIGN KEY (cod_caja) REFERENCES Caja (cod_caja),
	
	CONSTRAINT FK_nro_suc
	FOREIGN KEY (nro_suc) REFERENCES Sucursal (nro_suc)
	
)Engine = InnoDB;

CREATE TABLE ATM(

	cod_caja SMALLINT UNSIGNED NOT NULL,
	cod_postal INT UNSIGNED NOT NULL,
	direccion VARCHAR(45) NOT NULL,
	
	CONSTRAINT pk_atm
	PRIMARY KEY (cod_caja),
	
	CONSTRAINT FK_cod_caja_ATM
	FOREIGN KEY (cod_caja) REFERENCES Caja (cod_caja),
	
	CONSTRAINT FK_cod_postal
	FOREIGN KEY (cod_postal) REFERENCES Ciudad (cod_postal)
	
)Engine = InnoDB;

CREATE TABLE Transaccion(

	nro_trans INT UNSIGNED AUTO_INCREMENT NOT NULL,
	fecha DATE NOT NULL,
	hora TIME NOT NULL,
	monto DECIMAL(16,2) UNSIGNED NOT NULL,
	
	CONSTRAINT pk_transaccion
	PRIMARY KEY (nro_trans)
	
)Engine = InnoDB;

CREATE TABLE Debito(

	nro_trans INT UNSIGNED NOT NULL,
	descripcion TEXT,
	nro_cliente SMALLINT UNSIGNED NOT NULL,
	nro_ca INT UNSIGNED NOT NULL,
	
	CONSTRAINT pk_debito
	PRIMARY KEY (nro_trans),
	
	CONSTRAINT FK_nro_trans_D
	FOREIGN KEY (nro_trans) REFERENCES Transaccion (nro_trans),
	
	CONSTRAINT FK_nro_cliente_D
	FOREIGN KEY (nro_cliente,nro_ca) REFERENCES Cliente_ca (nro_cliente,nro_ca)
	
	
)Engine = InnoDB;

CREATE TABLE Transaccion_por_caja(

	nro_trans INT UNSIGNED NOT NULL,
	cod_caja SMALLINT UNSIGNED NOT NULL,
	
	CONSTRAINT pk_transaccion_por_caja
	PRIMARY KEY (nro_trans),
	
	CONSTRAINT FK_nro_trans_TPC
	FOREIGN KEY (nro_trans) REFERENCES Transaccion (nro_trans),
	
	CONSTRAINT FK_cod_caja_TPC
	FOREIGN KEY (cod_caja) REFERENCES Caja (cod_caja)
	
)Engine = InnoDB;

CREATE TABLE Deposito(

	nro_trans INT UNSIGNED NOT NULL,
	nro_ca INT UNSIGNED NOT NULL,
	
	CONSTRAINT pk_deposito
	PRIMARY KEY (nro_trans),
	
	CONSTRAINT FK_nro_trans_DEP
	FOREIGN KEY (nro_trans) REFERENCES Transaccion_por_caja (nro_trans),
	
	CONSTRAINT FK_cod_caja_DEP
	FOREIGN KEY (nro_ca) REFERENCES Caja_Ahorro (nro_ca)
	
)Engine = InnoDB;

CREATE TABLE Extraccion(

	nro_trans INT UNSIGNED NOT NULL,
	nro_cliente SMALLINT UNSIGNED NOT NULL,
	nro_ca INT UNSIGNED NOT NULL,
	
	CONSTRAINT pk_extraccion
	PRIMARY KEY (nro_trans),
	
	CONSTRAINT FK_nro_trans_E
	FOREIGN KEY (nro_trans) REFERENCES Transaccion_por_caja (nro_trans),
	
	CONSTRAINT FK_nro_cliente_E
	FOREIGN KEY (nro_cliente,nro_ca) REFERENCES Cliente_ca (nro_cliente,nro_ca)
	
)Engine = InnoDB;

CREATE TABLE Transferencia(

	nro_trans INT UNSIGNED NOT NULL,
	nro_cliente SMALLINT UNSIGNED NOT NULL,
	origen INT UNSIGNED NOT NULL,
	destino INT UNSIGNED NOT NULL,
	
	CONSTRAINT pk_transferencia
	PRIMARY KEY (nro_trans),
	
	CONSTRAINT FK_nro_trans_TR
	FOREIGN KEY (nro_trans) REFERENCES Transaccion_por_caja (nro_trans),
	
	CONSTRAINT FK_nro_cliente_TR
	FOREIGN KEY (nro_cliente,origen) REFERENCES Cliente_ca (nro_cliente,nro_ca),
	
	CONSTRAINT FK_destino
	FOREIGN KEY (destino) REFERENCES Caja_Ahorro (nro_ca)
	
)Engine = InnoDB;
	
#-------------------------------------------------------------------------
	
#Creacion de vistas
	
	CREATE VIEW datos_debito AS
	SELECT CA.nro_ca, CA.saldo, D.nro_trans, T.fecha, T.hora, "debito" AS tipo, T.monto, NULL AS destino, NULL AS cod_caja, C.nro_cliente, C.tipo_doc, C.nro_doc, C.nombre, C.apellido
	FROM (((Transaccion T JOIN Debito D ON T.nro_trans=D.nro_trans)
			JOIN Cliente_CA CCA ON (D.nro_cliente=CCA.nro_cliente AND D.nro_ca=CCA.nro_ca)) 
			JOIN Caja_Ahorro CA ON CCA.nro_ca=CA.nro_ca) 
			JOIN Cliente C ON CCA.nro_cliente=C.nro_cliente;
	
	CREATE VIEW datos_transferencia AS
	SELECT CA.nro_ca, CA.saldo, TR.nro_trans, T.fecha, T.hora, "transferencia" AS tipo, T.monto, TR.destino, TPC.cod_caja, C.nro_cliente, C.tipo_doc, C.nro_doc, C.nombre, C.apellido
	FROM ((((Transaccion T JOIN Transferencia TR ON T.nro_trans=TR.nro_trans)
			JOIN Cliente_CA CCA ON (TR.nro_cliente=CCA.nro_cliente AND CCA.nro_ca=TR.origen)) 
			JOIN Caja_Ahorro CA ON CCA.nro_ca=CA.nro_ca) 
			JOIN Cliente C ON CCA.nro_cliente=C.nro_cliente)
			JOIN Transaccion_por_caja TPC ON TPC.nro_trans=TR.nro_trans;
	
	CREATE VIEW datos_extraccion AS
	SELECT CA.nro_ca, CA.saldo, E.nro_trans, T.fecha, T.hora, "extraccion" AS tipo, T.monto, NULL AS destino, TPC.cod_caja, C.nro_cliente, C.tipo_doc, C.nro_doc, C.nombre, C.apellido
	FROM ((((Transaccion T JOIN Extraccion E ON T.nro_trans=E.nro_trans) 
			JOIN Cliente_CA CCA ON (E.nro_cliente=CCA.nro_cliente AND E.nro_ca=CCA.nro_ca)) 
			JOIN Caja_Ahorro CA ON CCA.nro_ca=CA.nro_ca) 
			JOIN Cliente C ON CCA.nro_cliente=C.nro_cliente)
			JOIN Transaccion_por_caja TPC ON TPC.nro_trans=E.nro_trans;
	
	CREATE VIEW datos_deposito AS
	SELECT CA.nro_ca, CA.saldo, D.nro_trans, T.fecha, T.hora, "deposito" AS tipo, T.monto, NULL AS destino, TPC.cod_caja, NULL AS nro_cliente, NULL AS tipo_doc, 
			NULL AS nro_doc, NULL AS nombre, NULL AS apellido
	FROM ((Transaccion T JOIN Deposito D ON T.nro_trans=D.nro_trans) 
			JOIN Caja_Ahorro CA ON D.nro_ca=CA.nro_ca)
			JOIN Transaccion_por_caja TPC ON TPC.nro_trans=D.nro_trans;
	
	CREATE VIEW trans_cajas_ahorro AS
	SELECT * FROM datos_debito UNION 
	SELECT * FROM datos_transferencia UNION
	SELECT * FROM datos_extraccion UNION 
	SELECT * FROM datos_deposito;
	
	#Numero de caja de ahorro
	#Saldo de caja de ahorro
	#Numero de transaccion
	#Fecha de transaccion
	#Hora de transaccion
	#Tipo de transaccion
	#Monto de transaccion
	
	#Si es Transferencia: Numero caja ahorro destino
	
	#Si es Transferencia, Extraccion o Deposito: Codigo de la caja de la Transaccion
	
	#Si es Transferencia, Extraccion o Debito: Numero de cliente
	#Si es Transferencia, Extraccion o Debito: Tipo de cliente
	#Si es Transferencia, Extraccion o Debito: Documento de cliente
	#Si es Transferencia, Extraccion o Debito: Nombre de cliente
	#Si es Transferencia, Extraccion o Debito: Apellido de cliente
	
#-------------------------------------------------------------------------

#Consulta trans_cajas_ahorro: se hace por partes, 4 vistas distintas, 1 para cada tipo de transacción. 
#Luego se agrupan en una sola vista

#Creacion de usuarios y otorgamiento de privilegios.


#Usuario admin con acceso total sobre todas las tablas y la posibilidad de crear usuarios y otorgar privilegios

	CREATE USER 'admin'@'localhost' IDENTIFIED BY 'admin';
	
	GRANT ALL PRIVILEGES ON banco.* TO 'admin'@'localhost' WITH GRANT OPTION;

#Usuario empleado

	CREATE USER 'empleado'@'%' IDENTIFIED BY 'empleado';
	
	GRANT SELECT ON banco.Empleado TO 'empleado'@'%';
    GRANT SELECT ON banco.Sucursal TO 'empleado'@'%';
    GRANT SELECT ON banco.Tasa_Plazo_Fijo TO 'empleado'@'%';
    GRANT SELECT ON banco.Tasa_Prestamo TO 'empleado'@'%';
	
	GRANT SELECT ON banco.Prestamo TO 'empleado'@'%';
    GRANT SELECT ON banco.Plazo_Fijo TO 'empleado'@'%';
    GRANT SELECT ON banco.Plazo_Cliente TO 'empleado'@'%';
    GRANT SELECT ON banco.Caja_Ahorro TO 'empleado'@'%';
    GRANT SELECT ON banco.Tarjeta TO 'empleado'@'%';
	
	GRANT INSERT ON banco.Prestamo TO 'empleado'@'%';
    GRANT INSERT ON banco.Plazo_Fijo TO 'empleado'@'%';
    GRANT INSERT ON banco.Plazo_Cliente TO 'empleado'@'%';
    GRANT INSERT ON banco.Caja_Ahorro TO 'empleado'@'%';
    GRANT INSERT ON banco.Tarjeta TO 'empleado'@'%';
	
	GRANT SELECT ON banco.Cliente_CA TO 'empleado'@'%';
    GRANT SELECT ON banco.Cliente TO 'empleado'@'%';
    GRANT SELECT ON banco.Pago TO 'empleado'@'%';
	
	GRANT INSERT ON banco.Cliente_CA TO 'empleado'@'%';
    GRANT INSERT ON banco.Cliente TO 'empleado'@'%';
    GRANT INSERT ON banco.Pago TO 'empleado'@'%';
	
	GRANT UPDATE ON banco.Cliente_CA TO 'empleado'@'%';
    GRANT UPDATE ON banco.Cliente TO 'empleado'@'%';
    GRANT UPDATE ON banco.Pago TO 'empleado'@'%';

#Usuario atm

	CREATE USER 'atm'@'%' IDENTIFIED BY 'atm';
	
	GRANT SELECT ON banco.trans_cajas_ahorro TO 'atm'@'%';
	
	GRANT SELECT ON banco.tarjeta TO 'atm'@'%';
	
	GRANT UPDATE ON banco.tarjeta TO 'atm'@'%';

	GRANT EXECUTE ON banco.* TO 'atm'@'%';

#-------------------------------------------------------------------------

delimiter ! # defino ! como delimitador
CREATE PROCEDURE RealizarTransferencia(IN MonT INT, IN N_Tarjeta BIGINT, IN N_CajaD SMALLINT, IN C_Caja SMALLINT)

	BEGIN
	
		# Declaro una variable local saldo_actual	
		DECLARE saldo_Actual DECIMAL(7,2);
		# Declaro variables locales para recuperar los errores 
		DECLARE codigo_SQL CHAR(5) DEFAULT '00000';	 
		DECLARE codigo_MYSQL INT DEFAULT 0;
		DECLARE mensaje_error TEXT;
		DECLARE N_Cliente SMALLINT;
		DECLARE N_CajaO SMALLINT;
		DECLARE EXIT HANDLER FOR SQLEXCEPTION
		BEGIN # En caso de una excepción SQLEXCEPTION retrocede la transacción y
         	  # devuelve el código de error especifico de MYSQL (MYSQL_ERRNO), 
			  # el código de error SQL (SQLSTATE) y el mensaje de error  	  
	          # "GET DIAGNOSTICS" solo disponible a partir de la versión 5.6.
			GET DIAGNOSTICS CONDITION 1 codigo_MYSQL= MYSQL_ERRNO, codigo_SQL= RETURNED_SQLSTATE, mensaje_error= MESSAGE_TEXT;
			SELECT 'SQLEXCEPTION!, transaccion abortada' AS resultado, codigo_MySQL, codigo_SQL, mensaje_error;		
			ROLLBACK;
		END;
		
         
		START TRANSACTION; # Comienza la transacción
		
			IF EXISTS (SELECT * FROM Tarjeta WHERE nro_tarjeta = N_Tarjeta) THEN
			
				IF EXISTS (SELECT * FROM Caja WHERE cod_caja = C_Caja) THEN
			
					IF EXISTS (SELECT * FROM Caja_Ahorro WHERE nro_ca = N_CajaD) THEN
					
						BEGIN
							
							SELECT nro_cliente INTO N_Cliente FROM Tarjeta WHERE nro_tarjeta = N_Tarjeta;
							IF EXISTS (SELECT * FROM Cliente WHERE nro_cliente = N_Cliente) THEN
							
								SELECT nro_ca INTO N_CajaO FROM Tarjeta WHERE nro_tarjeta = N_Tarjeta;
								IF EXISTS (SELECT * FROM Caja_Ahorro WHERE nro_ca = N_CajaO) THEN
								
									SELECT saldo INTO Saldo_Actual FROM Caja_Ahorro WHERE nro_ca = N_CajaO FOR UPDATE;
									# Recupero el saldo de la cuenta Origen en Saldo_Actual.
									# Al utilizar FOR UPDATE se indica que los datos involucrados en la
									# consulta van a ser actualizados luego.
									# De esta forma se obtiene un write_lock sobre estos datos, que se      
									# mantiene hasta que la trans. comete. Esto garantiza que nadie pueda
									# leer ni escribir el saldo de la cuenta de origen hasta que la trans. comete.      	    
				  
									IF Saldo_Actual >= MonT THEN
									
										BEGIN
						 
											UPDATE caja_ahorro SET saldo = (saldo - MonT) WHERE nro_ca = N_CajaO;
										 
											UPDATE caja_ahorro SET saldo = (saldo + MonT) WHERE nro_ca = N_CajaD;

											INSERT INTO transaccion(nro_trans,fecha,hora,monto) VALUES (NULL,CURDATE(),CURTIME(),MonT);
											
											INSERT INTO transaccion_por_caja(nro_trans,cod_caja) VALUES (LAST_INSERT_ID(),C_Caja);
											
											INSERT INTO transferencia(nro_trans,nro_cliente,origen,destino) VALUES (LAST_INSERT_ID(),N_Cliente,N_CajaO,N_CajaD);
											
											INSERT INTO transaccion(nro_trans,fecha,hora,monto) VALUES (NULL,CURDATE(),CURTIME(),MonT);
											
											INSERT INTO transaccion_por_caja(nro_trans,cod_caja) VALUES (LAST_INSERT_ID(),C_Caja);
											
											INSERT INTO deposito(nro_trans,nro_ca) VALUES (LAST_INSERT_ID(),N_CajaD);
											
											SELECT 'La transferencia se realizo con exito' AS resultado;
						 
										END;
										
									ELSE
										SELECT 'Saldo insuficiente para realizar la transferencia' AS resultado;
									END IF;
								
								ELSE
									SELECT 'ERROR: Caja ahorro origen inexistente' AS resultado;
								END IF;
								
							ELSE
								SELECT 'ERROR: Cliente inexistente' AS resultado;
							END IF;
			  
						END;
			  
					ELSE
						SELECT 'ERROR: Caja ahorro destino inexistente' AS resultado; 
					END IF;
					
				ELSE
					SELECT 'ERROR: Caja inexistente' AS resultado;
				END IF;
				
			ELSE
				SELECT 'ERROR: Tarjeta origen inexistente' AS resultado;
			END IF;	  
		
		COMMIT; # Comete la transacción
		
	END; !

#-------------------------------------------------------------------------

CREATE PROCEDURE RealizarExtraccion(IN MonT INT, IN N_Tarjeta BIGINT, IN C_Caja SMALLINT)

	BEGIN
	
		DECLARE Saldo_Actual DECIMAL(7,2); #Para obtener el saldo de la caja
		DECLARE codigo_SQL  CHAR(5) DEFAULT '00000';	 
		DECLARE codigo_MYSQL INT DEFAULT 0;
		DECLARE mensaje_error TEXT;
		DECLARE N_Cliente SMALLINT;
		DECLARE N_Caja SMALLINT;
		DECLARE EXIT HANDLER FOR SQLEXCEPTION 	 	 
		BEGIN # En caso de una excepción SQLEXCEPTION retrocede la transacción y
         	  # devuelve el código de error especifico de MYSQL (MYSQL_ERRNO), 
			  # el código de error SQL (SQLSTATE) y el mensaje de error  	  
	          # "GET DIAGNOSTICS" solo disponible a partir de la versión 5.6.
			GET DIAGNOSTICS CONDITION 1 codigo_MYSQL= MYSQL_ERRNO, codigo_SQL= RETURNED_SQLSTATE, mensaje_error= MESSAGE_TEXT;
			SELECT 'SQLEXCEPTION!, transaccion abortada' AS resultado, codigo_MySQL, codigo_SQL, mensaje_error;		
			ROLLBACK;
		END;		      
     

		START TRANSACTION; # Comienza la transacción
		
			IF EXISTS (SELECT * FROM Tarjeta WHERE nro_tarjeta = N_Tarjeta) THEN
			
				IF EXISTS (SELECT * FROM Caja WHERE cod_caja = C_Caja) THEN
				
					BEGIN
						
						SELECT nro_cliente INTO N_Cliente FROM Tarjeta WHERE nro_tarjeta = N_Tarjeta;
						IF EXISTS (SELECT * FROM Cliente WHERE nro_cliente = N_Cliente) THEN
						
							SELECT nro_ca INTO N_Caja FROM Tarjeta WHERE nro_tarjeta = N_Tarjeta;
							IF EXISTS (SELECT * FROM Caja_Ahorro WHERE nro_ca = N_Caja) THEN
							
								SELECT saldo INTO Saldo_Actual FROM Caja_Ahorro WHERE nro_ca = N_Caja FOR UPDATE;
							
								IF Saldo_Actual >= MonT THEN 
							
									BEGIN
							
										UPDATE caja_ahorro SET caja_ahorro.saldo = (saldo - MonT) WHERE nro_ca = N_Caja;
							  
										INSERT INTO transaccion(nro_trans,fecha,hora,monto) VALUES (NULL,CURDATE(),CURTIME(),MonT);
									
										INSERT INTO	transaccion_por_caja(nro_trans,cod_caja) VALUES (LAST_INSERT_ID(),C_Caja);
									
										INSERT INTO extraccion(nro_trans,nro_cliente,nro_ca) VALUES (LAST_INSERT_ID(),N_Cliente,N_Caja);
									
										SELECT 'La Extraccion se realizo con exito' AS resultado;
							
									END;
							
								ELSE 
									SELECT 'Saldo insuficiente para realizar la extraccion' AS resultado;
								END IF;
								
							ELSE
								SELECT 'ERROR: Caja ahorro inexistente' AS resultado;
							END IF;
							
						ELSE
							SELECT 'ERROR: Cliente inexistente' AS resultado;
						END IF;
						
					END;
			 
				ELSE
					SELECT 'ERROR: Caja inexistente' AS resultado;
				END IF;
			
			ELSE
				SELECT 'ERROR: Tarjeta inexistente' AS resultado;
			END IF;
			
		COMMIT; # Comete la transacción
		
	END; !

#-------------------------------------------------------------------------

CREATE TRIGGER CargoCuotas
AFTER INSERT ON Prestamo
FOR EACH ROW
	BEGIN
		DECLARE I INT DEFAULT 0;
		#Intento de conseguir el prestamo recien creado
		WHILE I < NEW.cant_meses DO
			INSERT INTO pago(nro_prestamo,nro_pago,fecha_venc,fecha_pago) VALUES (NEW.nro_prestamo,I+1,DATE_ADD(NEW.fecha,INTERVAL I+1 MONTH),NULL);
			SET I = I + 1;
		END WHILE;
	END; !

delimiter ; # devuelvo todo a la normalidad