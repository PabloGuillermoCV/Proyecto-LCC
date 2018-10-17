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
import java.sql.Types;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFormattedTextField;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;
import javax.swing.SwingConstants;
import javax.swing.WindowConstants;
import javax.swing.text.MaskFormatter;
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
	   private int Cod_Caja = 100;
	   private JButton BtnRealizarTransferencia;
	   private JButton BtnRealizarExtraccion;
	   private JTextField montos;
	   
	   public ConsultasATM() {
	      super();
	      initGUI();
	   }
	   
	   private void initGUI() {
	      try {
	    	 montos = new JTextField();
	         setPreferredSize(new Dimension(640, 480));
	         this.setBounds(0, 0, 881, 585);
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
	                  {
	                  	BtnRealizarTransferencia = new JButton("Realizar Transferencia");
	                  	pnlBotones.add(BtnRealizarTransferencia);
	                  	BtnRealizarTransferencia.addActionListener(new ActionListener() {
	                  		public void actionPerformed(ActionEvent evt) {
	                  			RealizarTrans();
	                  		}
	                  });
	               }
	                  {
	                  	BtnRealizarExtraccion = new JButton("Realizar Extraccion");
	                  	pnlBotones.add(BtnRealizarExtraccion);
	                  	BtnRealizarExtraccion.addActionListener(new ActionListener() {
	                  		public void actionPerformed(ActionEvent evt) {
	                  			RealizarExtra();
	                  		}

	                  	});
	                  }
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
	   /**
	    * Primer método para la Extreacción, obtengo el monto a extraer y delego la extracción en si en otro método
	    */
	   private void RealizarExtra() {
		   int okcx1 = JOptionPane.showConfirmDialog(null,montos,"Ingrese el Monto a Extraer", JOptionPane.OK_CANCEL_OPTION,JOptionPane.PLAIN_MESSAGE);
		   if(okcx1 == JOptionPane.OK_OPTION) {
			   int m = Integer.parseInt(montos.getText().trim());
			   String x = Extraer(m);
			   JOptionPane.showConfirmDialog(null,null,x,JOptionPane.OK_OPTION,JOptionPane.PLAIN_MESSAGE); //Devuelvo el resultado de la Extracción en un PopUp
		   }
	   }
	   
	   /**
	    * 
	    * @param m Monto a extraer de la tarjeta ingresada en el login
	    * @return un String que determina la natuiraleza de la Extracción, si fue exitosa o no
	    */
	   private String Extraer(int m) {
		   try {
			   int Out = 0;
			   Statement stm = this.conexionBD.createStatement();
			   //Aca asumo que el S.P existe y funciona, hay que primero hacer los S.P y corroborar que funcionan con MySQL primero
			   String sql = "call RealizarExtraccion(" + m + "," + Cod_Caja + "," + Out + ")";
			   stm.executeQuery(sql);
			   
		   }
		   catch(SQLException e) {
			   	//Devuelvo que la Transacción fallo a mi metodo padre para que lo maneje
                      return  "Se produjo un error al realizar la Extracción.\n" + e.getMessage();
		   }
		   return "Extracción Realizada con Exito";
	   }
	      
	   private void RealizarTrans() {
		   
	   }
	   
	   private void thisComponentShown(ComponentEvent evt) {
		  login ();
	      this.conectarBD ();
	      this.refrescarTabla ();
	   }
	   
	   /**
		 * Hace login de la Tarjeta por medio de Pop Ups
		 */
	   private void login () {
		   Card = new JPasswordField();
	       PIN_Field = new JPasswordField();
	       int okCxl = JOptionPane.showConfirmDialog(null, Card, "Ingresar nro de Tarjeta", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
	       if (okCxl == JOptionPane.OK_OPTION) {
	           Tarj = new String (Card.getPassword());
	       }
		   int okCx2 = JOptionPane.showConfirmDialog(null, PIN_Field, "Ingresar Pin", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
		   if (okCx2 == JOptionPane.OK_OPTION) {
			   Pin = new String (PIN_Field.getPassword());
		   }
	   }
	   
	   private void btnUltimosMoviminetosActionPerformed(ActionEvent evt) {
			//Mostrar las ultimas 15 transacciones
		   try {
			   Statement stm = this.conexionBD.createStatement();
			   String sql = "SELECT TCA.fecha, TCA.hora, TCA.monto, TCA.tipo, TCA.cod_caja AS CodigoCaja, TCA.destino"
			   		+ " FROM trans_cajas_ahorro TCA NATURAL JOIN Tarjeta T"
			   		+ " WHERE " + Tarj + " = T.nro_tarjeta AND T.PIN = md5(" + Pin + ")"
			   		+ " ORDER BY TCA.fecha DESC, TCA.hora DESC"
			   		+ " LIMIT 15";
			   stm.execute(sql);
			   ResultSet R = stm.getResultSet();
			   tabla.refresh(R);
			   stm.close();
			   R.close();
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
		   try {
			   Statement stmt = this.conexionBD.createStatement();
		         String sql = "SELECT DISTINCT saldo"
		                      + " FROM trans_cajas_ahorro TCA NATURAL JOIN Tarjeta T"
		                      + " WHERE T.nro_tarjeta = " + Tarj + " AND T.PIN = md5( " + Pin + ")";
		         stmt.execute(sql);
		         ResultSet R = stmt.getResultSet();
		         int sal = 0;
		         if (R.next()) {
		        	 sal = R.getInt(1);
		         }
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
				   Statement stmt = this.conexionBD.createStatement();
			         String sql = "SELECT fecha, hora, tipo, monto, cod_caja AS codCaja, destino"
			                      + " FROM trans_cajas_ahorro TCA"
			                      + " WHERE TCA.fecha >= " + txtFechaInicio.getText().trim() + " AND TCA.fecha <= " + txtFechaFin.getText().trim();
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
		         
		         tabla.getColumnByDatabaseName("hora").setType(Types.CHAR);
		         
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
		      else if (!Fechas.validar(this.txtFechaInicio.getText().trim()))
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