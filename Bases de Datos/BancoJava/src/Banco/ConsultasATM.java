package Banco;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFormattedTextField;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.SwingConstants;
import javax.swing.WindowConstants;
import javax.swing.text.MaskFormatter;
import EjemploClase.Fechas;
import quick.dbtable.DBTable;

@SuppressWarnings("serial")
public class ConsultasATM extends javax.swing.JInternalFrame {
	   
	   private JPanel pnlInferior;
	   private JFormattedTextField txtFechaInicio; 
	   private JFormattedTextField txtFechaFin;
	   private JLabel lblFecha;
	   private JLabel lblNombre;
	   private JButton btnSaldo;
	   private JButton btnUltimosMovimientos;
	   private JButton btnMovimientosPeriodo;
	   private JPanel pnlBotones;
	   private JPanel pnlCampos;
	   private DBTable tabla = new DBTable(); 
	   private JPasswordField PIN_Field;
	   private JPasswordField Card;
	   private String Tarj,Pin;
	   private Connection conexionBD = null;
	   
	   public ConsultasATM() {
	      super();
	      initGUI();
	   }
	   
	   private void initGUI() {
	      try {
	         setPreferredSize(new Dimension(640, 480));
	         this.setBounds(0, 0, 640, 480);
	         this.setTitle("Consultas ATM");
	         this.setDefaultCloseOperation(WindowConstants.HIDE_ON_CLOSE);
	         this.setResizable(true);
	         this.setClosable(true);
	         this.setVisible(true);
	         BorderLayout thisLayout = new BorderLayout();
	         this.setMaximizable(true);
	         getContentPane().setLayout(thisLayout);
	         this.addComponentListener(new ComponentAdapter() {
	            public void componentHidden(ComponentEvent evt) {
	               thisComponentHidden(evt);
	            }
	            public void componentShown(ComponentEvent evt) {
	               thisComponentShown(evt);
	            }
	         });
	         
	         pnlInferior = new JPanel();
	            BorderLayout jPanel1Layout = new BorderLayout();
	            jPanel1Layout.setHgap(3);
	            jPanel1Layout.setVgap(3);
	            getContentPane().add(pnlInferior, BorderLayout.SOUTH);
	            pnlInferior.setLayout(jPanel1Layout);
	            pnlInferior.setPreferredSize(new java.awt.Dimension(638, 88));
	            {
	               pnlCampos = new JPanel();
	               GridLayout jPanel2Layout = new GridLayout(2, 2);
	               jPanel2Layout.setRows(2);
	               jPanel2Layout.setColumns(2);
	               jPanel2Layout.setHgap(5);
	               jPanel2Layout.setVgap(3);
	               pnlInferior.add(pnlCampos, BorderLayout.CENTER);
	               pnlCampos.setLayout(jPanel2Layout);
	               pnlCampos.setPreferredSize(new java.awt.Dimension(638, 76));
	               pnlCampos.setBorder(BorderFactory.createEmptyBorder(3, 0, 0, 0));
	               {
	                  lblNombre = new JLabel();
	                  pnlCampos.add(lblNombre);
	                  lblNombre.setText("Fecha Inicio");
	                  lblNombre.setHorizontalAlignment(SwingConstants.TRAILING);
	               }
	               {
	                  txtFechaInicio = new JFormattedTextField(new MaskFormatter("##'/##'/####"));
	                  pnlCampos.add(txtFechaInicio);
	                  txtFechaInicio.setPreferredSize(new java.awt.Dimension(638, 41));
	               }
	               {
	                  lblFecha = new JLabel();
	                  pnlCampos.add(lblFecha);
	                  lblFecha.setText("Fecha Fin");
	                  lblFecha.setHorizontalAlignment(SwingConstants.TRAILING);
	               }
	               {
	                  txtFechaFin = new JFormattedTextField(new MaskFormatter("##'/##'/####"));
	                  pnlCampos.add(txtFechaFin);
	               }
	            }
	         
	       //Utilizando DBTable            
	            {
	            	// crea la tabla  
	            	tabla = new DBTable();    
	            	
	            	// Agregar la tabla al frame (no necesita JScrollPane como Jtable)
	                getContentPane().add(tabla, BorderLayout.CENTER);           
	                          
	               //setea la tabla solo para lectura, no se puede editar su contenido  
	               tabla.setEditable(false); 
	            }
	            pnlBotones = new JPanel();
	               pnlInferior.add(pnlBotones, BorderLayout.SOUTH);
	               pnlBotones.setPreferredSize(new java.awt.Dimension(638, 31));
	               {
	                  btnSaldo = new JButton();
	                  pnlBotones.add(btnSaldo);
	                  btnSaldo.setText("Consultar Saldo");
	                  btnSaldo.addActionListener(new ActionListener() {
	                     public void actionPerformed(ActionEvent evt) {
	                        btnSaldoActionPerformed(evt);
	                     }

						
	                  });
	               }
	               {
	                  btnUltimosMovimientos = new JButton();
	                  pnlBotones.add(btnUltimosMovimientos);
	                  btnUltimosMovimientos.setText("Mostrar Ultimos Movimientos (ultimos 15 dias)");
	                  btnUltimosMovimientos.addActionListener(new ActionListener() {
	                     public void actionPerformed(ActionEvent evt) {
	                        btnUltimosMoviminetosActionPerformed(evt);
	                     }

						
	                  });
	               }
	               {
	                  btnMovimientosPeriodo = new JButton();
	                  pnlBotones.add(btnMovimientosPeriodo);
	                  btnMovimientosPeriodo.setText("Consultar Movimientos por Periodo");
	                  btnMovimientosPeriodo.addActionListener(new ActionListener() {
	                     public void actionPerformed(ActionEvent evt) {
	                        btnMovimientosPeriodoActionPerformed(evt);
	                     }

						
	                  });
	               }
	      }
	      catch(Exception e) {
	    	  e.printStackTrace();
	      }
	   }
	   
	   private void thisComponentShown(ComponentEvent evt) {
		  login ();
	      this.conectarBD ();
	      this.refrescarTabla ();
	   }
	   
	   /**
		 * Hace login del Empleado por medio de Pop Ups
		 */
	   private void login () {
		   Card = new JPasswordField();
	       PIN_Field = new JPasswordField();
	       int okCxl = JOptionPane.showConfirmDialog(null, Card, "Ingresar nro de Tarjeta", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
	       if (okCxl == JOptionPane.OK_OPTION) {
	           Tarj = Card.getPassword().toString(); 
	       }
		   int okCx2 = JOptionPane.showConfirmDialog(null, PIN_Field, "Ingresar Pin", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
		   if (okCx2 == JOptionPane.OK_OPTION) {
			   Pin = PIN_Field.getPassword().toString(); 
		   }
	   }
	   
	   private void btnUltimosMoviminetosActionPerformed(ActionEvent evt) {
			//Mostrar las ultimas 15 transacciones
		   try {
			   //corroborar Query
			   //PIN esta con md5, hay que ver como ejecutarlo aca -> ya deberia de estar
			   //El NATURAL JOIN deberia ser correcto, estoy corroborando al ejecutar las queries en SQL
			   //lo unico que dudo es el "AS CodigoCaja"... no me estaria funcionando
			   Statement stm = this.conexionBD.createStatement();
			   String sql = "SELECT  fecha, hora, monto, tipo, (cod_caja AS CodigoCaja), destino"
			   		+ "FROM  (trans_cajas_ahorro NATURAL JOIN tarjeta) AS x"
			   		+ "WHERE " + Tarj + " = x.nro_tarjeta AND x.PIN = md5(" + Pin + ")"
			   		+ "ORDER BY fecha DESC , hora DESC"
			   		+ "LIMIT 15";
			   stm.execute(sql);
			   ResultSet R = stm.getResultSet();
			   tabla.refresh(R);
			   stm.close();
			   R.close();
			
			   /* int i = 1; Ver esto, aunque no se si se puede iterar manualmente con una DBTable 
			   while(i <= 15 && R.next() != false) {
				   R.
			   }
			   */
			   
			   
		   }
		   catch(SQLException ex) {
			   JOptionPane.showMessageDialog(this,
                       "Se produjo un error al mostrar los ultimos movimientos.\n" + ex.getMessage(),
                       "Error",
                       JOptionPane.ERROR_MESSAGE);
		   }
		}
	   
	   
	   private void btnSaldoActionPerformed(ActionEvent evt) {
		   //Mostrar el Saldo del Usuario
		   //El query deberia funcionar
		   try {
			   Statement stmt = this.conexionBD.createStatement();
		         String sql = "SELECT DISTINCT saldo" +
		                      "FROM trans_cajas_ahorro natural join tarjeta"
		                      + "WHERE tarjeta.nro_tarjeta = " + Tarj + "and tarjeta.PIN = md5( " + Pin + ")" ;
		         stmt.execute(sql);
		         ResultSet R = stmt.getResultSet();
		         int sal = R.getInt(1);
		         //No necesito mostrar una tabla, solo piden el saldo de la tarjeta, lo muestro en un pop up
		         JOptionPane.showMessageDialog(this, "El saldo de la Tarjeta es: " + sal);
		         stmt.close();
		         R.close();
		   }
		   catch(SQLException ex) {
			   JOptionPane.showMessageDialog(this,
                       "Se produjo un error al mostrar el saldo.\n" + ex.getMessage(),
                       "Error",
                       JOptionPane.ERROR_MESSAGE);
		   }
			
		}
	   
	   private void btnMovimientosPeriodoActionPerformed(ActionEvent evt) {
		   if(validarCampos()) {
			   try {
				   //Revisar Query, se me confunde un toque con tanta informacion en el View
				   Statement stmt = this.conexionBD.createStatement();
			         String sql = "SELECT Fecha,hora,tipo,monto,cod_caja AS codCaja,destino" +
			                      "FROM trans_cajas_ahorro"
			                      + "WHERE Fecha >= " + txtFechaInicio.getText().trim() + "and Fecha <=" + txtFechaFin.getText().trim() ;
			         stmt.execute(sql);
			         ResultSet R = stmt.getResultSet();
			         tabla.refresh(R); //Obtenidas las tuplas, refresco la tabla con el ResultSet
			         stmt.close();
			         R.close();
			   }
			   catch(SQLException ex) {
				   JOptionPane.showMessageDialog(this,
	                       "Se produjo un error al mostrar los movimientos.\n" + ex.getMessage(),
	                       "Error",
	                       JOptionPane.ERROR_MESSAGE);
			   }
		  }
			
		}
	   
	   private void thisComponentHidden(ComponentEvent evt) 
	   {
		  //No solo me desconecto, tambien limpio los datos del que ingreso, asi otro usuario puede ingresar
	      this.desconectarBD();
	      Tarj = "";
	      Pin = "";
	   }
	   
	   private void conectarBD() {
		      if (this.conexionBD == null) {
		         try {
		            Class.forName("com.mysql.cj.jdbc.Driver").newInstance();
		         }
		         catch (Exception ex) {
		            System.out.println(ex.getMessage());
		         }
		        
		         try {
		        	String driver ="com.mysql.cj.jdbc.Driver";
		            String usuario = "atm";
		            String password = "atm";
		            String urlConexion = "jdbc:mysql://localhost/banco?serverTimezone=UTC&useSSL=false&allowPublicKeyRetrieval=true";
		            tabla.connectDatabase (driver, urlConexion, usuario, password);
		            conexionBD = DriverManager.getConnection (urlConexion, usuario, password);
		         }
		         catch (SQLException ex) {
		            JOptionPane.showMessageDialog(this,
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

		   private void desconectarBD() {
		       if (this.conexionBD != null) {
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
		   
		 //Revisar, no me parece necesario todo el codigo que este metodo contiene 
		 //Se usa la primera vez para mostrar todos los campos, medio raro de tener 
		 private void refrescarTabla() {
		      try {
		         Statement stmt = this.conexionBD.createStatement();

		         String sql = "SELECT *" + 
		                      "FROM trans_cajas_ahorro " +
		                      "ORDER BY nro_ca DESC";


		         ResultSet rs = stmt.executeQuery(sql);
		       
		                 
		        //actualiza el contenido de la tabla con los datos del result set rs
		         tabla.refresh(rs);

		        //setea el formato de visualizacion de la columna "fecha" a dia/mes/ano
		         tabla.getColumnByDatabaseName("fecha").setDateFormat("dd/MM/YYYY");
		         
		         tabla.getColumnByDatabaseName("fecha").setMinWidth(80);       
		         rs.close();
		         stmt.close();
		      }
		      catch (SQLException ex) {
		         System.out.println("SQLException: " + ex.getMessage());
		         System.out.println("SQLState: " + ex.getSQLState());
		         System.out.println("VendorError: " + ex.getErrorCode());
		      }
		      
		      this.inicializarCampos();
		   }
		 
		 private void inicializarCampos() {
		      this.txtFechaFin.setText("");
		      this.txtFechaInicio.setText("");
		   }
		 
		 //Esta funcion es necesaria para validar el input ANTES de realizar los query
		 private boolean validarCampos() {
		      String mensajeError = null;
		      if (this.txtFechaInicio.getText().isEmpty())
		      {
		         mensajeError = "Debe ingresar un valor para el campo 'Fecha Inicio'.";
		      }
		      else if (this.txtFechaFin.getText().isEmpty())
		      {
		         mensajeError = "Debe ingresar un valor para el campo 'Fecha Fin'.";
		      }
		      else if (! Fechas.validar(this.txtFechaInicio.getText().trim()))
		      {
		         mensajeError = "En el campo 'Fecha Inicio' debe ingresar un valor con el formato dd/mm/aaaa.";
		      }
		      else if (!Fechas.validar(this.txtFechaFin.getText().trim())){
		    	  mensajeError = "En el campo 'Fecha Fin' debe ingresar un valor con el formato dd/mm/aaaa.";
		      }

		      if (mensajeError != null)
		      {
		         JOptionPane.showMessageDialog(this,
		                                       mensajeError,
		                                       "Error",
		                                       JOptionPane.ERROR_MESSAGE);
		         return false;
		      }
		      return true;
		   }
}