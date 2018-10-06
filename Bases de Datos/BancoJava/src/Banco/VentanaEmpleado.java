package Banco;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.sql.Types;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.GregorianCalendar;
import java.sql.Connection;
import java.sql.Date;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;
import javax.swing.border.BevelBorder;
import javax.swing.table.DefaultTableModel;

import quick.dbtable.*;
import javax.swing.JLabel;
import java.awt.Font;
import javax.swing.SwingConstants;
import java.awt.Rectangle;
import javax.swing.JList;

@SuppressWarnings({ "serial", "unused" })
public class VentanaEmpleado extends javax.swing.JInternalFrame {
	
	private JPanel pnlConsulta;
	private JTextArea txtConsulta;
	private JButton botonBorrar;
	private JButton btnEjecutar;   
	private JScrollPane scrConsulta;
	private String legajo;
	private String clave;
	private JPasswordField pf;
	private JTextField nf;
	private JTextField Num_doc;
	private JTextField Tipo_Doc;
	private DBTable TablaEmpleado;
	private String nro;
	private String tipo;
	protected Connection conexionBD = null;
	
	public VentanaEmpleado () {
		super ();
		setTitle("Consultas Empleado");
		getContentPane().setLayout(null);
		
		
		JPanel PaneTabla = new JPanel();
		PaneTabla.setBounds(0, 161, 248, 326);
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
		
		JButton CrearPrest = new JButton("Crear Prestamo");
		CrearPrest.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg1) {
				CrearPrest(arg1);
			}
		});
		CrearPrest.setBounds(20, 114, 107, 23);
		getContentPane().add(CrearPrest);
		
		JButton CuotasBtn = new JButton("Ver Cuotas");
		CuotasBtn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				verCuotas(arg0);
			}
		});
		CuotasBtn.setBounds(366, 114, 107, 23);
		getContentPane().add(CuotasBtn);
		
		JButton btnVerMorosos = new JButton("Ver Morosos");
		btnVerMorosos.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				verMor(e);
			}
		});
		btnVerMorosos.setBounds(672, 114, 107, 23);
		getContentPane().add(btnVerMorosos);
		
		TablaEmpleado = new DBTable();
		PaneTabla.add(TablaEmpleado);
		
		JPanel Panel_Cuotas = new JPanel();
		Panel_Cuotas.setBounds(258, 161, 235, 326);
		getContentPane().add(Panel_Cuotas);
		
		JList Lista_Cuotas = new JList();
		Panel_Cuotas.add(Lista_Cuotas);
		}
	
	//Métodos donde irian las consultas SQL
	
	/**
	 * llena la Tabla de Clientes Morosos
	 * @param e Action Event del botón, no se usa pa mucho
	 */
	private void verMor(ActionEvent e) {
		//Necesito los datos del cliente (Nro Cliente, Tipo y Nro de Doc, Apellido y nombre)
		//Para cada cliente, neceisto los Prestamos en discordia (Nro de Prest, monto, cant_meses y valor cuota
		//para cada Prestamo, necesito saber la cantidad de cuotas atrasadas (tienen que ser al menos 2)
		try {
			Statement stmt = this.conexionBD.createStatement();
			String SQL1 = "SELECT nro_cliente, tipo_doc , nro_doc, apellido, nombre, nro_prestamo, monto, cant_meses,valor_cuota, COUNT(nro_pago)"
					+ "FROM (cliente NATURAL JOIN prestamo) AS x, pago "
					+ "WHERE x.nro_prestamo = pago.nro_prestamo and COUNT(nro_pago) >= 2"; // , verificar
			ResultSet rs = stmt.executeQuery(SQL1);
			TablaEmpleado.refresh(rs);

			//para que está esto de abajo? lo de arriba ^ deberia hacer todo automáticamente
			//Aunque DBTable no tiene SetAutoCreateRowSorter, ver esto
	        int I = 0;
	        while (rs.next()) {
	            TablaEmpleado.setValueAt(rs.getInt("nro_cliente"), I, 0);
	            TablaEmpleado.setValueAt(rs.getString("tipo_doc"), I, 1);
	            TablaEmpleado.setValueAt(rs.getInt("nro_doc"), I, 2);
	            I++;
	        }
			
			rs.close();
	        stmt.close();
		}
		catch (SQLException e1) {
			e1.printStackTrace();
		}
	}
	
	/**
	 * llena la Lista de Cuotas para un cliente
	 * @param e ActionEvent del botón, no se usa pa mucho, hay que agarrar los textos de los JTextField
	 */
	private void verCuotas(ActionEvent e) {
		nro = Num_doc.getText();
		tipo = Tipo_Doc.getText();
		
		
	}
	
	/**
	 * Intenta crear un prestamo para un cliente
	 * @param e ActionEvent del Botón, ne se usa para mucho, hay que agarrar los textos de los JTextField
	 */
	private void CrearPrest(ActionEvent e) {
		nro = Num_doc.getText();
		tipo = Tipo_Doc.getText();
		try {
			Statement stmt = this.conexionBD.createStatement();
			ResultSet R;
			String dia,mes,anio;
			noPrestActual(nro, tipo);
		}
		catch(SQLException f) {
			
		}
	}
	
	private void thisComponentShown (ComponentEvent evt) {
		login();
		this.conectarBD();
	}
	
	private void noPrestActual(String doc, String typeDoc) {
		int mon, mes;
		try {
			Statement stmt = this.conexionBD.createStatement();
			ResultSet R;
			//SQL para determinar los prestamos actuales de un cliente
			R = stmt.executeQuery("SELECT nro_prestamo FROM prestamo WHERE nro_cliente = " + doc) ;
			if(!R.first()) { //si no hay prestamos vigentes (la primer columna del Query es vacia, por ende no hay filas) 
				R = stmt.executeQuery("SELECT MAX(periodo) FROM tasa_prestamo");
				mes = R.getInt(1);
				R = stmt.executeQuery("SELECT MAX(monto_sup) from tasa_prestamo");
				mon = R.getInt(1); //Ojo, es un decimal, no se si anda
				IngresoPrest ven = new IngresoPrest();
				int m = ven.getmon();
				int p = ven.getper();
				if(corroborar(m,p,mes,mon)) { //Los montos ingresados no superan los máximos establecidos por las tasas
					EjecutarCreacion(m,p);
				}
			}
			stmt.close();
			R.close();
			
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
	
	//Corroboro que lo ingresado no supere los máximos de las Tasas
	private boolean corroborar(int mon, int mes,int monSup,int perSup) {
		if(mon < monSup && mes < perSup)
			return true;
		else
			return false;
	}
	
	//Con todo corroborado, creo el Prestamo
	//Esto implica crear el prestamo y todas las cuotas asociadas al mismo
	//cantidad de cuotas (Pagos en con fecha_pago = NULL) depende de la cantidad de Meses
	//usar date_add(<Fecha_actual_mientras_se_Itera>, interval 1 month)
	private void EjecutarCreacion(int plata, int periodo) {
		int i = 2;
		try {
			Statement stmt = this.conexionBD.createStatement();
			ResultSet R;
			//cargo el Prestamo
			int tasa_Int;
			int Interes;
			int ValCuota;
			R = stmt.executeQuery("SELECT tasa FROM tasa_prestamo where tasa_prestamo.periodo = " + periodo); //revisar si esta bien
			tasa_Int= R.getInt(1);
			Interes = (plata + tasa_Int + periodo)/1200;
			ValCuota = (plata + tasa_Int)/periodo;
			R = stmt.executeQuery("SELECT nro_cliente FROM Cliente WHERE nro_doc = " + nro + "and tipo_doc = " + tipo);
			int c = R.getInt(1);
			 DateTimeFormatter dtf = DateTimeFormatter.ofPattern("yyyy/MM/dd");  
			 LocalDateTime now = LocalDateTime.now(); 
			 String d = now.toString();
			stmt.executeUpdate("INSERT INTO prestamo (fecha,cant_meses,monto,tasa_interes,interes,"
					+ "valor_cuota,legajo,nro_cliente) VALUES"
					+ "STR_TO_DATE(" + d + ",\"%d-%m-%Y\")," + periodo + "," + plata + "," +
					tasa_Int + "," + Interes + "," + ValCuota + "," + legajo + "," + c); //corroborar que los datos son correctos
			
			//Para cargar las cuotas necesito el nro de prestamo del prestamo recién creado
			R = stmt.executeQuery("SELECT nro_prestamo FROM prestamo WHERE nro_cliente =" + c);
			int nro_pre = R.getInt(1);
			cargarCuotas(c,d,nro_pre,periodo);
			
			stmt.close();
			R.close();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		
	}
	
	//Método auxiliar para cargar las cuotas
	private void cargarCuotas(int c, String d, int nro_pre, int periodo) {
		Statement stmt;
		try {
			stmt = this.conexionBD.createStatement();
		 		ResultSet R;
			int i = 2;
			//Cargo la primera cuota
			stmt.executeUpdate("INSERT INTO Pago (nro_prestamo,nro_pago,fecha_venc,fecha_pago) VALUES"
					+ nro_pre + "," + 1 + ",STR_TO_DATE(" + d +",%d-%m-%Y)" + ", NULL");
			//Cargo el resto de las cuotas
			while(i <= periodo) {
				R = stmt.executeQuery("SELECT DATE_ADD(" + d + "INTERVAL 1 month)");
				//Aca cargo las cuotas una por una
				stmt.executeUpdate("INSERT INTO Pago (nro_prestamo,nro_pago,fecha_venc,fecha_pago) VALUES"
						+ nro_pre + "," + i + ",STR_TO_DATE(" + d + ",%d-%m-%Y)" + ", NULL)"); 
				//Fecha_Pago = NULL ya que es una cuota que NO se ha pagado todavia
				d = R.getString(1);
			}
		}
		catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	/**
	 * Hace login del Empleado por medio de Pop Ups
	 */
	private void login() {
		final JFrame parent = new JFrame();
        JButton button = new JButton();
        parent.getContentPane().add(button);
        parent.pack();
        parent.setVisible(true);

        button.addActionListener(new java.awt.event.ActionListener() {
            @Override
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                legajo = JOptionPane.showInputDialog(parent,
                        "Ingrese numero de Legajo", null);
            }
        });
		int okCxl = JOptionPane.showConfirmDialog(null, pf, "Enter Password", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
		if (okCxl == JOptionPane.OK_OPTION) {
			clave = pf.getPassword().toString(); 
		}
	}
	   
	private void thisComponentHidden (ComponentEvent evt) {
	    this.desconectarBD();
	}

	private void btnEjecutarActionPerformed (ActionEvent evt) {
	    this.refrescarTabla();      
    }
	
	private void conectarBD () {
		try {
	        String driver ="com.mysql.cj.jdbc.Driver";
	        String servidor = "%";
	        String baseDatos = "banco";
	        //String legajo = "empleado";
	        //String clave = "empleado";
	        String uriConexion = "jdbc:mysql://" + servidor + "/" + baseDatos+"?serverTimezone=UTC";
	        //Establece una conexion con la  B.D. "banco"  usando directamante una tabla DBTable    
	        TablaEmpleado.connectDatabase (driver, uriConexion, legajo, clave);   
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
	
	private void desconectarBD () {
		try {
			TablaEmpleado.close();
	    }
	    catch (SQLException ex) {
	        System.out.println("SQLException: " + ex.getMessage());
	        System.out.println("SQLState: " + ex.getSQLState());
	        System.out.println("VendorError: " + ex.getErrorCode());
	    }
	}
	
	private void refrescarTabla () {
	    try {    
	          
	    	// seteamos la consulta a partir de la cual se obtendran los datos para llenar la tabla
	    	TablaEmpleado.setSelectSql(this.txtConsulta.getText().trim());
	    	// obtenemos el modelo de la tabla a partir de la consulta para 
	    	// modificar la forma en que se muestran de algunas columnas  
	    	TablaEmpleado.createColumnModelFromQuery();    	    
	    	for (int i = 0; i < TablaEmpleado.getColumnCount(); i++) { 
	    	    // para que muestre correctamente los valores de tipo TIME (hora)  		   		  
	    		if (TablaEmpleado.getColumn(i).getType()==Types.TIME) {    		 
	    			TablaEmpleado.getColumn(i).setType(Types.CHAR);  
	  	       	}
	    		// cambiar el formato en que se muestran los valores de tipo DATE
	    		if (TablaEmpleado.getColumn(i).getType()==Types.DATE) {
	    			TablaEmpleado.getColumn(i).setDateFormat("dd/MM/YYYY");
	    		}
	        }  
	    	// actualizamos el contenido de la tabla.   	     	  
	    	TablaEmpleado.refresh();
	    	// No es necesario establecer  una conexion, crear una sentencia y recuperar el 
	    	// resultado en un resultSet, esto lo hace automaticamente la tabla (DBTable) a 
	    	// patir de la conexion y la consulta seteadas con connectDatabase() y 
	        // setSelectSql() respectivamente.
	    }
	    catch (SQLException ex)
	    {
	        // en caso de error, se muestra la causa en la consola
	        System.out.println("SQLException: " + ex.getMessage());
	        System.out.println("SQLState: " + ex.getSQLState());
	        System.out.println("VendorError: " + ex.getErrorCode());
	        JOptionPane.showMessageDialog(SwingUtilities.getWindowAncestor(this),
	                            ex.getMessage() + "\n", 
	                            "Error al ejecutar la consulta.",
	                            JOptionPane.ERROR_MESSAGE);
	         
	    }
    }
}