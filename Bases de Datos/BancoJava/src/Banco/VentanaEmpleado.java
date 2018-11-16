package Banco;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import javax.swing.JButton;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import quick.dbtable.*;
import javax.swing.JLabel;
import java.awt.Font;
import javax.swing.SwingConstants;
import javax.swing.JScrollBar;
import java.awt.BorderLayout;

@SuppressWarnings({"serial","unused"})
public class VentanaEmpleado extends javax.swing.JInternalFrame {
	
	private JPanel pnlConsulta;
	private JButton botonBorrar;
	private JScrollPane scrConsulta;
	private String legajo;
	private String clave;
	private JTextField Leg;
	private JPasswordField pf;
	private JTextField nf;
	private JTextField Num_doc;
	private JTextField Tipo_Doc;
	private DBTable tabla = new DBTable();
	private int nro;
	private String tipo;
	private boolean Salir = false;
	protected Connection conexionBD = null;
	private int seleccionado = -1;
	private JButton btnRegistrarPago;
	
	
	public VentanaEmpleado () {
		super ();
		setClosable(true);
		setResizable(true);
		setMaximizable(true);
		setTitle("Consultas Empleado");
		getContentPane().setLayout(null);
		this.addComponentListener(new ComponentAdapter() {
            public void componentHidden(ComponentEvent evt) {
                thisComponentHidden(evt);
            }
            public void componentShown(ComponentEvent evt) {
                thisComponentShown(evt);
            }
        });
		
		JPanel PaneTabla = new JPanel();
		PaneTabla.setBounds(0, 161, 875, 560);
		getContentPane().add(PaneTabla);
			
		Num_doc = new JTextField();
		Num_doc.setBounds(10, 83, 129, 20);
		getContentPane().add(Num_doc);
		Num_doc.setColumns(10);
		
		JLabel Label_Doc = new JLabel("Numero de Documento");
		Label_Doc.setHorizontalAlignment(SwingConstants.CENTER);
		Label_Doc.setFont(new Font("Tahoma", Font.BOLD, 11));
		Label_Doc.setBounds(10, 58, 129, 14);
		getContentPane().add(Label_Doc);
		
		JLabel Label_tipo_doc = new JLabel("Tipo de Documento");
		Label_tipo_doc.setHorizontalAlignment(SwingConstants.CENTER);
		Label_tipo_doc.setFont(new Font("Tahoma", Font.BOLD, 11));
		Label_tipo_doc.setBounds(662, 58, 122, 14);
		getContentPane().add(Label_tipo_doc);
		
		Tipo_Doc = new JTextField();
		Tipo_Doc.setBounds(672, 83, 112, 20);
		getContentPane().add(Tipo_Doc);
		Tipo_Doc.setColumns(10);
		
		btnRegistrarPago = new JButton("Registrar Pago");
		btnRegistrarPago.setEnabled(false);
		btnRegistrarPago.setBounds(361, 54, 112, 23);
		getContentPane().add(btnRegistrarPago);
		btnRegistrarPago.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				seleccionarFila();
			}
		});
		
		PaneTabla.add(tabla);
		//hago que NO pueda ser seleccionable por defecto
		tabla.setEnabled(true);
		tabla.setEditable(false);
		
		JScrollBar scrollBar = new JScrollBar();
		tabla.add(scrollBar, BorderLayout.WEST);
		tabla.addMouseListener(new MouseAdapter() {
			public void MouseListener(MouseEvent evt) {
				TablaClick(evt);
			}
		});
		
		JButton CrearPrest = new JButton("Crear Prestamo");
		CrearPrest.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg1) {
				btnRegistrarPago.setEnabled(false);
				tabla.setEnabled(false);
				CrearPrest(arg1);
			}
		});
		CrearPrest.setBounds(20, 114, 107, 23);
		getContentPane().add(CrearPrest);
		
		JButton CuotasBtn = new JButton("Ver Cuotas");
		CuotasBtn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				tabla.setEnabled(true);
				btnRegistrarPago.setEnabled(true);
				verCuotas(arg0);
			}
		});
		CuotasBtn.setBounds(361, 114, 112, 23);
		getContentPane().add(CuotasBtn);
		
		JButton btnVerMorosos = new JButton("Ver Morosos");
		btnVerMorosos.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				btnRegistrarPago.setEnabled(false);
				tabla.setEnabled(false);
				verMor(e);
			}
		});
		btnVerMorosos.setBounds(672, 114, 107, 23);
		getContentPane().add(btnVerMorosos);
		
		}

	private void TablaClick(MouseEvent evt) {
		if ((this.tabla.getSelectedRow() != -1) && (evt.getClickCount() == 2)){
			seleccionarFila();
	    }
	}
	
	private void seleccionarFila() {
		try {
			this.seleccionado = this.tabla.getSelectedRow();
			if (tabla.getRowCount()>0) {
				//Obtengo los valores en la Tabla por la fila que se selecciono
				String nroCuota = tabla.getValueAt(seleccionado, 0).toString().trim();
				String valor = tabla.getValueAt(seleccionado, 1).toString().trim();
				//hago un pop up para que se confirme, si se confirma, se registra el pago
				int ok = JOptionPane.showConfirmDialog(null, "Desea registrar el pago de la cuota numero " + nroCuota + " con valor " + valor + "?","Aviso", JOptionPane.OK_CANCEL_OPTION,JOptionPane.PLAIN_MESSAGE);
				if(ok == JOptionPane.OK_OPTION) {
					registrarPago(nroCuota,valor);
				}
			}
			else {
				JOptionPane.showMessageDialog(this, "No quedan mas cuotas por pagar.", "Error", JOptionPane.ERROR_MESSAGE);
			}
		}
		catch(ArrayIndexOutOfBoundsException e) {
			System.out.println(e.getMessage());
		}
		catch(NullPointerException e) {
			JOptionPane.showMessageDialog(this, "No quedan mas cuotas por pagar.", "Error", JOptionPane.ERROR_MESSAGE);
		}
	}
	
	private void registrarPago(String nroC, String money) {
		try {
			LocalDateTime now = LocalDateTime.now();
			DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
			String d = now.format(formatter);
			Statement stmt = this.conexionBD.createStatement();
			//Hago un UPDATE Query
			stmt.executeUpdate("UPDATE Pago P SET P.fecha_pago = '" + d + "' WHERE P.nro_pago = " + nroC);
			//Si devuelve un ResultSet la cosa me devuelve -1
			if(stmt.getUpdateCount() != -1) {
				//El Update tuvo exito
				Object[] Opcion = {"OK"};
				JOptionPane.showOptionDialog(null, "Se ha registrado el Pago de la Cuota con exito","Exito",JOptionPane.PLAIN_MESSAGE,JOptionPane.QUESTION_MESSAGE,null,Opcion,Opcion[0]);
				//Refresco la tabla para que se note que se pago la cuota
				ResultSet R = stmt.executeQuery("SELECT PA.nro_pago AS Cuota_Nro, PR.valor_cuota AS Valor, PA.fecha_venc AS Vencimiento FROM Prestamo PR NATURAL JOIN Pago PA NATURAL JOIN Cliente C WHERE C.tipo_doc = '" + tipo + "' AND C.nro_doc = " + nro + " AND PA.fecha_pago is NULL");
				tabla.refresh(R);
				stmt.close();
				R.close();
			}
		} catch (SQLException e) {
			System.out.println("SQLException: " + e.getMessage());
	        System.out.println("SQLState: " + e.getSQLState());
	        System.out.println("VendorError: " + e.getErrorCode());
			e.printStackTrace();
		}
	}
	
	//Metodos donde irian las consultas SQL
	/**
	 * llena la Tabla con Clientes Morosos
	 * @param e Action Event del boton, no se usa pa mucho
	 */
	private void verMor(ActionEvent e) {
		//Necesito los datos del cliente (Nro Cliente, Tipo y Nro de Doc, Apellido y nombre)
		//Para cada cliente, neceisto los Prestamos en discordia (Nro de Prest, monto, cant_meses y valor cuota
		//para cada Prestamo, necesito saber la cantidad de cuotas atrasadas (tienen que ser al menos 2)
		try {
			LocalDateTime now = LocalDateTime.now();
			DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
			String fechaAInsertar = now.format(formatter);
			Statement stmt = this.conexionBD.createStatement();
			String SQL1 = "SELECT C.nro_cliente, C.tipo_doc , C.nro_doc, C.apellido, C.nombre, PR.nro_prestamo, PR.monto, PR.cant_meses, PR.valor_cuota, COUNT(PA.nro_pago) AS cant_cuotas_atrasadas FROM (Cliente C JOIN Prestamo PR ON C.nro_cliente = PR.nro_cliente) JOIN Pago PA ON PR.nro_prestamo = PA.nro_prestamo WHERE PA.fecha_pago IS NULL AND PA.fecha_venc < '" + fechaAInsertar + "' GROUP BY C.nro_cliente, PR.nro_prestamo HAVING COUNT(PA.fecha_pago IS NULL) > 1";
			ResultSet R = stmt.executeQuery(SQL1);
			tabla.refresh(R);
			
			R.close();
	        stmt.close();
		}
		catch (SQLException e1) {
			System.out.println("SQLException: " + e1.getMessage());
	        System.out.println("SQLState: " + e1.getSQLState());
	        System.out.println("VendorError: " + e1.getErrorCode());
			e1.printStackTrace();
		}
	}
	
	/**
	 * llena la Lista de Cuotas para un cliente
	 * @param e ActionEvent del botón, no se usa pa mucho, hay que agarrar los textos de los JTextField
	 */
	private void verCuotas(ActionEvent e) {
		try {
			nro = Integer.parseInt(Num_doc.getText());
			tipo = Tipo_Doc.getText();
			Num_doc.setText("");
			Tipo_Doc.setText("");
			Statement stmt = this.conexionBD.createStatement();
			ResultSet R;
			R = stmt.executeQuery("SELECT PA.nro_pago AS Cuota_Nro, PR.valor_cuota AS Valor, PA.fecha_venc AS Vencimiento FROM Prestamo PR NATURAL JOIN Pago PA NATURAL JOIN Cliente C WHERE C.tipo_doc = '" + tipo + "' AND C.nro_doc = '" + nro + "' AND PA.fecha_pago is NULL");
			tabla.refresh(R);
			stmt.close();
			R.close();
		}
		catch(SQLException f) {
			System.out.println("SQLException: " + f.getMessage());
		    System.out.println("SQLState: " + f.getSQLState());
		    System.out.println("VendorError: " + f.getErrorCode());
			f.printStackTrace();
		}
		catch(NumberFormatException ex) {
			btnRegistrarPago.setEnabled(false);
			JOptionPane.showMessageDialog(this, "Numero o Tipo de documento es vacio", "Error", JOptionPane.ERROR_MESSAGE);
		}
	}
	
	/**
	 * Intenta crear un prestamo para un cliente
	 * @param e ActionEvent del Boton, no se usa para mucho, hay que agarrar los textos de los JTextField
	 */
	private void CrearPrest(ActionEvent e) {
		try {
			nro = Integer.parseInt(Num_doc.getText());
			tipo = Tipo_Doc.getText();
			noPrestActual(nro, tipo);
		}
		catch(NumberFormatException ex) {
			btnRegistrarPago.setEnabled(false);
			JOptionPane.showMessageDialog(this, "Numero o Tipo de documento es vacio", "Error", JOptionPane.ERROR_MESSAGE);
		}
	}

	
	/**
	 * Metodo que verifica si el cliente ingresado tiene un Prestamo en vigencia
	 * Caso contrario, se avanza en la creacion de un nuevo prestamo para dicho cliente
	 * @param doc numero de documento del cliente
	 * @param typeDoc tipo del documento del cliente
	 */
	private void noPrestActual(int doc, String typeDoc) {
		int mon = 0;
		int mes = 0;
		try {
			Statement stmt = this.conexionBD.createStatement();
			ResultSet R;
			//SQL para determinar los prestamos actuales de un cliente, Verificar SQL
			R = stmt.executeQuery("SELECT P.nro_prestamo FROM Prestamo P NATURAL JOIN Pago X WHERE P.nro_cliente = " + doc + "AND fecha_pago IS NULL "); 
			if(!R.next()) { //si no hay prestamos vigentes (la primer columna del Query es vacia, por ende no hay filas) 
				
				int m = 0;
				int p = 0;
				
				JTextField Leg1 = new JTextField();
		        int okCx2 = JOptionPane.showConfirmDialog(null,Leg1,"Ingrese monto a prestar", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
		        if(okCx2 == JOptionPane.OK_OPTION) {
		        	m = Integer.parseInt(Leg1.getText().trim());
		        }
		        
		        JTextField Leg2 = new JTextField();
		        int okCx1 = JOptionPane.showConfirmDialog(null,Leg2,"Ingrese periodo en meses", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
		        if(okCx1 == JOptionPane.OK_OPTION) {
		        	p = Integer.parseInt(Leg2.getText().trim());
		        }
		        
		        //Pregunto si el periodo de meses que me ingresaron ES ALGUNO DE LOS DISPONIBLES
		        //Tambien verifico que dado ese periodo, el monto ingresado este ENTRE
		        	//un Monto Inferior Y superior PARA DICHO PERIODO
		        //NO solo corroboro que el periodo y el monto sean legales, sino que de una obtengo la tasa asociada
		        //Al nuevo prestamo con los datos dados
		        //BETWEEN hace comparacion <= / >=, asi que no hay riesgo de que me ingresen un monto limite y la cosa no ande
		        R = stmt.executeQuery("SELECT TP.tasa FROM Tasa_Prestamo TP WHERE TP.periodo = " + p + " AND " + m + " BETWEEN TP.monto_inf AND TP.monto_sup");
		        
		        if (R.next()) {
		        	int t = R.getInt(1);
		        	EjecutarCreacion(m,p,t);
		        }
		        else {//Si el ResutSet es vacio, es poque le erraron al monto o al periodo, comunico dicho error
		        	JOptionPane.showConfirmDialog(null, "Ocurrio un error al obtener la Tasa, el Monto o supera los $20000 o el Periodo NO es un Periodo de meses valido","Error",JOptionPane.OK_OPTION, JOptionPane.ERROR_MESSAGE);
		        }
				
			}
			else {
				//Muestro un cuadro de error que especifica que el Cliente ya tiene un prestamo
				JOptionPane.showConfirmDialog(null, "No es posible crear un prestamo ya que el cliente ya posee un prestamo a su nombre", "Error", JOptionPane.OK_OPTION, JOptionPane.ERROR_MESSAGE);
			}
			stmt.close();
			R.close();
			
		} 
		catch (SQLException e) {
			System.out.println("SQLException: " + e.getMessage());
	        System.out.println("SQLState: " + e.getSQLState());
	        System.out.println("VendorError: " + e.getErrorCode());
			e.printStackTrace();
		}
		catch (NumberFormatException e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
	}
	
	//Con todo corroborado, creo el Prestamo
	//Esto implica crear el prestamo y todas las cuotas asociadas al mismo
	//cantidad de cuotas (Pagos en con fecha_pago = NULL) depende de la cantidad de Meses
	//usar date_add(<Fecha_actual_mientras_se_Itera>, interval 1 month)
	private void EjecutarCreacion(int plata, int periodo, int tasa) {
		int i = 2;
		try {
			Statement stmt1 = this.conexionBD.createStatement();
			Statement stmt2 = this.conexionBD.createStatement();
			ResultSet R1;
			ResultSet R2;
			//cargo el Prestamo
			int Interes;
			int ValCuota;
			int intC = 0;
			Interes = (plata + tasa + periodo)/1200;
			ValCuota = (plata + tasa)/periodo;
			String sql = "SELECT C.nro_cliente FROM Cliente C WHERE C.nro_doc = " + nro + " AND C.tipo_doc = '" + tipo + "'";
			R1 = stmt1.executeQuery(sql);
			if (R1.next()) {
				intC = R1.getInt(1);
			}
			LocalDateTime now = LocalDateTime.now(); 
			DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
			String fechaAInsertar = now.format(formatter);
			sql = "INSERT INTO prestamo (fecha,cant_meses,monto,tasa_interes,interes,valor_cuota,legajo,nro_cliente) VALUES ('" + fechaAInsertar + "'," + periodo + "," + plata + "," + tasa + "," + Interes + "," + ValCuota + "," + legajo + "," + intC + ")";
			stmt1.executeUpdate(sql);
			stmt1.close();
			R1.close();
			/*
			//Para cargar las cuotas necesito el nro de prestamo del prestamo recien creado
			sql = "SELECT P.nro_prestamo FROM prestamo P WHERE P.nro_cliente = " + intC;
			stmt2 = this.conexionBD.createStatement();
			R2 = stmt2.executeQuery(sql);
			int nro_pre = 0;
			if (R2.next()) {
				nro_pre = R2.getInt(1);
			}
			//ESTE METODO DEBE BORRARSE UNA VEZ QUE SE CORROBORE QUE EL TRIGGER DE CUOTAS FUNCIONA!!!!!!!
			cargarCuotas(intC,fechaAInsertar,nro_pre,periodo);
			
			int nro = -10;
			//Para corroborar que las cosas se cargaron bien, hago dos consultas:
			//La primera obtiene si se cargo el nuevo prestamo obteniendo el ultimo ID insertado que coincida
			//COn el legajo del empleado, el cliente que lo solicito y cuya fecha de creacion sea ahora
			R2 = stmt2.executeQuery("SELECT nro_prestamo FROM prestamo WHERE nro_prestamo = LAST_INSERT_ID(nro_Prestamo) AND legajo = " + legajo + " AND nro_cliente = " + intC + "AND fecha = CURDATE()");
			nro = R2.getInt(1);
			if(nro != -10) {
				//En la segunda, corroboro que se hayan cargado las cuotas, cuento cuantas cuotas se crearon para el Prestamo que cree
				//Y corroboro que sea la cantidad correcta de cuotas
				R2 = stmt2.executeQuery("SELECT COUNT(nro_prestamo) FROM pago WHERE nro_prestamo = " + nro);
				nro = R2.getInt(1);
				*/
				if(nro == periodo) {
					JOptionPane.showConfirmDialog(null, "Se ha cargado el Prestamo y sus cuotas asociadas correctamente", 
							"Carga de Prestamo exitosa",JOptionPane.OK_OPTION, JOptionPane.PLAIN_MESSAGE);
				}
				else {
					JOptionPane.showConfirmDialog(null, "Ha ocurrido un error al cargar las cuotas asociadas al prestamo", 
							"Carga de Prestamo erronea",JOptionPane.OK_OPTION, JOptionPane.PLAIN_MESSAGE);
				}
			nro = -10;
		} 
		catch (SQLException e) {
			System.out.println("SQLException: " + e.getMessage());
	        System.out.println("SQLState: " + e.getSQLState());
	        System.out.println("VendorError: " + e.getErrorCode());
			e.printStackTrace();
		}
	}
	
	
	//Metodo auxiliar para cargar las cuotas
	//DEBE  "Borrarse" UNA VEZ QUE EL TRIGGER DE LAS CUOTAS ANDE!
	private void cargarCuotas(int c, String fechaD, int nro_pre, int periodo) {
		Statement stmt;
		try {
			stmt = this.conexionBD.createStatement();
		 	String sql;
			int i = 2;
			//Cargo la primera cuota
			stmt.executeUpdate("INSERT INTO Pago (nro_prestamo,nro_pago,fecha_venc,fecha_pago) VALUES (" + nro_pre + "," + 1 + ",'" + fechaD + "', NULL)");
			//Cargo el resto de las cuotas
			while (i <= periodo) {
				ResultSet R;
				sql = "SELECT DATE_ADD('" + fechaD + "', INTERVAL 1 MONTH)";
				R = stmt.executeQuery(sql);
				//Aca cargo las cuotas una por una
				//Obtengo la nueva fecha de vencimiento producto de mover la fecha 1 mes adelante
				String d = R.getString(0);
				sql = "INSERT INTO Pago (nro_prestamo,nro_pago,fecha_venc,fecha_pago) VALUES (" + nro_pre + "," + i + ",'" + d + "', NULL)";
				stmt.executeUpdate(sql); 
				fechaD = d;
				//Fecha_Pago = NULL ya que es una cuota que NO se ha pagado todavia
			}
			stmt.close();
		}
		catch (SQLException e) {
			System.out.println("SQLException: " + e.getMessage());
	        System.out.println("SQLState: " + e.getSQLState());
	        System.out.println("VendorError: " + e.getErrorCode());
			e.printStackTrace();
		}
	}

	   /**
	    * Metodo que se ejecuta cuando este componente aparece en pantalla
	    * @param evt el evento que crea esto
	    */
		private void thisComponentShown (ComponentEvent evt) {
			boolean Verif = false;
			this.conectarBD(); //Conecto a la Vista
			while(!Verif) {
				login(); //Obtengo los datos del empleado
				if (legajo == "" || clave == "") {
			        int Reply = JOptionPane.showConfirmDialog(null,"El Legajo y Password no pueden ser vacios, Ingrese los datos nuevamente","Error",JOptionPane.YES_NO_OPTION,JOptionPane.ERROR_MESSAGE);
					if (Reply == JOptionPane.NO_OPTION) {
						Salir = true;
					}
			    }
				else {
					Verif = VerificarLogin();
				}
				if (Salir == true) {
					thisComponentHidden(evt);
					System.exit(0);
				}
			}
		}

	   /**
	    * Metodo privado que Verifica que los datos de Login ingresados sean correctos
	    * @return un valor booleano que discrimina si el loguo fue hecho con exito o no
	    */
       private boolean VerificarLogin() {
			boolean ret = true;
			try {
				Statement st = this.conexionBD.createStatement();
				String sql = "SELECT E.legajo, E.password FROM Empleado E WHERE E.legajo = " + legajo + " AND E.password = md5('" +  clave + "')";
				ResultSet R = st.executeQuery(sql);
				
				ret = R.next(); //Pregunto si el ResultSet tiene un dato
				if (!ret) {
					//Hago pop-ups para decir que fallo
					int reply = JOptionPane.showConfirmDialog(null, "Ocurrio un error al buscar su usuario, por favor, ingrese los datos nuevamente","Error",JOptionPane.YES_NO_OPTION,JOptionPane.ERROR_MESSAGE);
					if (reply == JOptionPane.NO_OPTION) {
						Salir = true;
					}
				}
			}
			catch (SQLException e) {
				e.printStackTrace();
			}
			return ret;
		}
	
	/**
	 * Hace login del Empleado por medio de Pop Ups
	 */
	private void login() {
		legajo = "";
		clave = "";
		Leg = new JTextField();
        int okCx2 = JOptionPane.showConfirmDialog(null,Leg,"Ingrese numero de Legajo", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
        if(okCx2 == JOptionPane.OK_OPTION && !Leg.getText().isEmpty()) {
        	legajo = Leg.getText().trim();
        }
	    pf = new JPasswordField();
		int okCxl = JOptionPane.showConfirmDialog(null, pf, "Enter Password", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
		if (okCxl == JOptionPane.OK_OPTION && pf.getPassword().length > 0) {
			clave = new String (pf.getPassword());
		}
	}
	

   /**
    * Metodo que se ejecuta cuando el componente se hace invisible
    * @param evt evento que cause la ida del componente
    */
	private void thisComponentHidden (ComponentEvent evt) {
	    //Desconecto y  limpio todos los atributos
		this.desconectarBD();
	    legajo = "";
	    clave = "";
	    tipo = "";
	    seleccionado = -1;
	}
	
	private void conectarBD () {
		if (conexionBD == null) {
			try {
		        String driver ="com.mysql.cj.jdbc.Driver";
		        String usuario = "empleado";
	            String password = "empleado";
		        String urlConexion = "jdbc:mysql://localhost/banco?serverTimezone=America/Argentina/Buenos_Aires&useSSL=false&allowPublicKeyRetrieval=true";
		        //Establece una conexion con la  B.D. "banco"  usando directamante una tabla DBTable    
		        tabla.connectDatabase (driver, urlConexion, usuario, password);
		        conexionBD = DriverManager.getConnection (urlConexion, usuario, password);
		    }
		    catch (SQLException ex) {
		        JOptionPane.showMessageDialog (this,
		        			"Se produjo un error al intentar conectarse a la base de datos.\n" + ex.getMessage(),
		                    "Error",
		                    JOptionPane.ERROR_MESSAGE);
		        System.out.println("SQLException: " + ex.getMessage());
		        System.out.println("SQLState: " + ex.getSQLState());
		        System.out.println("VendorError: " + ex.getErrorCode());
		    }
		    catch (ClassNotFoundException e) {
		        e.printStackTrace();
		    }
		}
	}
	
	private void desconectarBD () {
		if (conexionBD != null) {
			try {
				tabla.close();
				conexionBD.close();
				conexionBD = null;
		    }
		    catch (SQLException ex) {
		        System.out.println("SQLException: " + ex.getMessage());
		        System.out.println("SQLState: " + ex.getSQLState());
		        System.out.println("VendorError: " + ex.getErrorCode());
		    }
		}
	}
}