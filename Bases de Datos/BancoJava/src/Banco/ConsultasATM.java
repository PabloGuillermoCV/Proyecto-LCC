package Banco;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.sql.Connection;
import java.util.Date;
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
import javax.swing.SwingUtilities;
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
	   private JButton BtnRealizarTransferencia;
	   private JButton BtnRealizarExtraccion;
	   private JTextField montos;
	   private boolean Salir = false;
	   
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
	                  lblNombre.setText("Fecha Inicio (D/M/A)");
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
	                  lblFecha.setText("Fecha Fin (D/M/A)");
	                  lblFecha.setHorizontalAlignment(SwingConstants.TRAILING);
	               }
	               {
	                  txtFechaFin = new JFormattedTextField(new MaskFormatter("##'/##'/####"));
	                  pnlCampos.add(txtFechaFin);
	               }
	            }
	         
	       //Utilizando DBTable            
	            {
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
	                  btnUltimosMovimientos.setText("Mostrar Ultimos Movimientos");
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
	    * Primer metodo para la Extreaccion, obtengo el monto a extraer 
	    * y delego la extraccion en si en otro metodo
	    */
	   private void RealizarExtra() {
		   int okcx1 = JOptionPane.showConfirmDialog(null,montos,"Ingrese el Monto a Extraer", JOptionPane.OK_CANCEL_OPTION,JOptionPane.PLAIN_MESSAGE);
		   if(okcx1 == JOptionPane.OK_OPTION) {
			   int m = Integer.parseInt(montos.getText().trim());
			   String x = Extraer(m);
			 //Devuelvo el resultado de la Extracción en un PopUp
			   JOptionPane.showConfirmDialog(null,x,"Fin Extraccion",JOptionPane.OK_OPTION,JOptionPane.PLAIN_MESSAGE); 
		   }
	   }
	   
	   /**
	    * Metodo para el Proyecto 3 que realiza una extracción de la cuenta del banco
	    * @param m Monto a extraer de la tarjeta ingresada en el login
	    * @return un String que determina la naturaleza de la Extracción, si fue exitosa o no
	    */
	   private String Extraer(int monto) {
		  String ret ="";
		   try {
			   Statement stm = this.conexionBD.createStatement();
			   ResultSet R;
			   //Aca asumo que el S.P existe y funciona, hay que primero hacer los S.P y corroborar que funcionan con MySQL primero
			   String sql = "call RealizarExtraccion(" + monto + "," + Tarj + ",100)";
			   R = stm.executeQuery(sql);
			   if(R.next()) {
				   ret = R.getString(1);
			   }
			   R.close();
			   stm.close();
			   
		   }
		   catch(SQLException e) {
			   e.printStackTrace();
			   	//Devuelvo que la Transaccion fallo a mi metodo padre para que lo maneje
                      return  "Se produjo un error al realizar la Extraccion.\n" 
			   			+ e.getMessage() + "\n" + "SQL State: " + e.getSQLState() + "\n" 
                      + "Error Code: " + e.getErrorCode();
		   }
		   return ret;
	   }
	   
	   /**
	    * Metodo del Proyecto 3 que Realiza una Transferencia entre dos cuentas
	    * Este metodo obtiene los datos necesarios y delega la Transferencia en si a otro metodo
	    */
	   private void RealizarTrans() {
		   int CajaD = 0;
		   int mon = 0;
		   //Obtengo el monto a transferir y la caja destino con Pop-ups
		   int okcx1 = JOptionPane.showConfirmDialog(null,montos,"Ingrese el Monto a Transferir", 
				   		JOptionPane.OK_CANCEL_OPTION,JOptionPane.PLAIN_MESSAGE);
		   if(okcx1 == JOptionPane.OK_OPTION)
			   mon = Integer.parseInt(montos.getText().trim());
		   JTextField C = new JTextField();
		   okcx1 = JOptionPane.showConfirmDialog(null, C, "Ingrese el codigo de la caja a la cual transferir"
				   	,JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
		   if(okcx1 == JOptionPane.OK_OPTION)
			   	CajaD = Integer.parseInt(C.getText().trim());
		   
		   Transferir(CajaD,mon);
		   
	   }
	   
	   /**
	    * metodo que realiza la llamada a procedimiento SQL y realiza la transferencia
	    * @param CajaD Numero de la Caja de ahorro destino
	    * @param plataT monto a transferir
	    */
	   private void Transferir(int CajaD, int monto) {
		   try {
			String ret = "";
			Statement stmt = conexionBD.createStatement();
			ResultSet R = stmt.executeQuery("call RealizarTransferencia(" + monto + "," + Tarj + "," + CajaD + ",100)");
			if(R.next()) {
				ret = R.getString(1);
			}
			R.close();
			stmt.close();
			JOptionPane.showConfirmDialog(null, ret, "Fin Transferencia", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
		} catch (SQLException e) {
	    	  JOptionPane.showMessageDialog(this,
                      "Se produjo un error al intentar Realizar la Transacción.\n" + e.getMessage(),
                      "Error",
                      JOptionPane.ERROR_MESSAGE);
				System.out.println("SQLException: " + e.getMessage());
				System.out.println("SQLState: " + e.getSQLState());
				System.out.println("VendorError: " + e.getErrorCode());
		}
		   
	   }
	   
	   /**
	    * Metodo que genera los ultimos 15 movimientos de un cliente ingresado
	    * @param evt evento del boton asociado, no se usa, honestamente
	    */
	   private void btnUltimosMoviminetosActionPerformed(ActionEvent evt) {
		   try {
			   Statement stm = this.conexionBD.createStatement();
			   //Query para obtener los movimientos, limitamos a los primeros 15 resultados
			   String sql = "SELECT TCA.fecha, TCA.hora, TCA.monto, TCA.tipo, TCA.cod_caja AS CodigoCaja, TCA.destino FROM trans_cajas_ahorro TCA JOIN Tarjeta T ON TCA.nro_ca = T.nro_ca WHERE T.nro_tarjeta = " + Tarj + " ORDER BY TCA.fecha DESC, TCA.hora DESC LIMIT 15";
			   refrescarTabla(sql); //Delego el refresco de la DBTable en el metodo especifico
			   stm.close(); //Cierro y el Statement
		   }
		   catch(SQLException ex) {
			   JOptionPane.showMessageDialog(this,
                       "Se produjo un error al mostrar los ultimos movimientos.\n" + ex.getMessage(),
                       "Error",
                       JOptionPane.ERROR_MESSAGE);
		   }
		}
	   
	   /**
	    * Metodo que obtiene el Saldo del cliente ingresado
	    * @param evt evento del boton asociado, No se usa, honestamente
	    */
	   private void btnSaldoActionPerformed(ActionEvent evt) {
		   //Mostrar el Saldo del Usuario
		   try {
			   Statement stmt = this.conexionBD.createStatement();
		         String sql = "SELECT DISTINCT saldo FROM trans_cajas_ahorro TCA NATURAL JOIN Tarjeta T WHERE T.nro_tarjeta = " + Tarj + " AND T.PIN = md5('" + Pin + "')";
		         stmt.execute(sql);
		         ResultSet R = stmt.getResultSet();
		         int sal = 0;
		         if (R.next()) {
		        	 sal = R.getInt(1); //Pregunto si hay resultados y en caso afirmativo obtengo el saldo
		         }
		         else {//Si el ResultSet era vacio, me mandaron cualquier cosa, tiro pop-up de error
		        	 JOptionPane.showConfirmDialog(null, null, "EL Usuario Ingresado NO tiene saldo,"
		        	 		+ " verifique que los datos ingresados son correctos", JOptionPane.OK_OPTION,JOptionPane.ERROR_MESSAGE);
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
	   
	   /**
	    * Metodo que realiza la consulta de Movimientos por Periodo
	    * @param evt evento del boton asociado, no se usa, honestamente
	    */
	   private void btnMovimientosPeriodoActionPerformed(ActionEvent evt) {
		   if (validarCampos()) {
			   try {
				   //Pasa la fecha ingresada por pantalla a un formato para que pueda ser comparada con la BD
				   String Ini = this.txtFechaInicio.getText().trim();
				   String Fin = this.txtFechaFin.getText().trim();
				   Date IDate = Fechas.convertirStringADate(Ini);
				   Date FDate = Fechas.convertirStringADate(Fin);
				   String I = Fechas.convertirDateAStringDB(IDate);
				   String F = Fechas.convertirDateAStringDB(FDate);
				   Statement stmt = this.conexionBD.createStatement();
			       String sql = "SELECT TCA.fecha, TCA.hora, TCA.tipo, TCA.monto, TCA.cod_caja AS codCaja, TCA.destino FROM trans_cajas_ahorro TCA NATURAL JOIN Tarjeta T WHERE TCA.fecha >= '" + I + "' AND TCA.fecha <= '" + F + "' AND T.nro_tarjeta = " + Tarj + " AND T.PIN = md5('" + Pin + "') ORDER BY nro_ca DESC";
			       refrescarTabla(sql); //Delego el Refresco en el metodo que lo hace como la gente
			       stmt.close();
			   }
			   catch(SQLException ex) {
				   JOptionPane.showMessageDialog(this, "Se produjo un error al mostrar los movimientos.\n" + ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
			   }
		   }
		}
	   
	   /**
	    * Metodo que se ejecuta cuando este componente aparece en pantalla
	    * @param evt el evento que crea esto
	    */
	   private void thisComponentShown(ComponentEvent evt) {
		      boolean Verif = false;
		      boolean ok = true;
			  this.conectarBD ();
		      while (!Verif & ok) {
					login(); //Obtengo los datos del empleado
					//Verifico si alguno de los campos del login fueron vacios
					if (Tarj == "" || Pin == "") {
						int Reply = JOptionPane.showConfirmDialog(null, "Alguno de los campos del login eran vacios, por favor, ingrese los datos nuevamente","Error" , JOptionPane.YES_NO_OPTION, JOptionPane.ERROR_MESSAGE);
						if (Reply == JOptionPane.NO_OPTION)
							Salir = true;
					}
					else {
						Verif = VerificarLogin();
					}
					if (Salir == true) {
						thisComponentHidden(evt);
						System.exit(0);
					}
			  }
		     
	    	  if (ok) { //Aqui tengo que verificar si realmente entre bien al romper forzadamaente el ciclo while
			      //Muestro datos de las transacciones del cliente
			      String sql = "SELECT fecha, hora, tipo, monto, cod_caja AS codCaja, destino FROM trans_cajas_ahorro TCA NATURAL JOIN Tarjeta T WHERE T.nro_tarjeta = " + Tarj + " AND T.PIN = md5('" + Pin + "') ORDER BY nro_ca DESC";
			      this.refrescarTabla (sql);
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
				String sql = "SELECT nro_tarjeta, PIN FROM Tarjeta T WHERE T.nro_tarjeta = " + Tarj + " AND T.PIN = md5('" +  Pin + "')";
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
		 * Hace login de la Tarjeta por medio de Pop Ups
		 */
	   private void login () {
		   Tarj = "";
		   Pin = "";
		   Card = new JPasswordField();
	       int okCxl = JOptionPane.showConfirmDialog(null, Card, "Ingresar nro de Tarjeta", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
	       if (okCxl == JOptionPane.OK_OPTION && Card.getPassword().length > 0) {
	           Tarj = new String (Card.getPassword());
	       }
	       PIN_Field = new JPasswordField();
		   int okCx2 = JOptionPane.showConfirmDialog(null, PIN_Field, "Ingresar Pin", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
		   if (okCx2 == JOptionPane.OK_OPTION && PIN_Field.getPassword().length > 0) {
			   Pin = new String (PIN_Field.getPassword());
		   }
	   }
	   
	   /**
	    * Metodo que se ejecuta cuando el componente se hace invisible
	    * @param evt evento que cause la ida del componente
	    */
	   private void thisComponentHidden(ComponentEvent evt) {
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
		            String urlConexion = "jdbc:mysql://localhost/banco?serverTimezone=America/Argentina/Buenos_Aires&useSSL=false&allowPublicKeyRetrieval=true";
		            tabla.connectDatabase (driver, urlConexion, usuario, password);
		            conexionBD = DriverManager.getConnection (urlConexion, usuario, password);
		         }
		         catch (SQLException ex) {
		        	 ex.printStackTrace();
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
		   
		 
		 private void inicializarCampos() {
		      this.txtFechaFin.setText("");
		      this.txtFechaInicio.setText("");
		   }
		 
		 //Esta funcion es necesaria para validar el input ANTES de realizar los query
		 private boolean validarCampos() {
			  String Fin = this.txtFechaFin.getText().trim();
			  String Ini = this.txtFechaInicio.getText().trim();
			  Date F = Fechas.convertirStringADate(Fin);
			  Date I = Fechas.convertirStringADate(Ini);
		      String mensajeError = null;
		      if (Ini == "")
		      {
		         mensajeError = "Debe ingresar un valor para el campo 'Fecha Inicio'.";
		      }
		      else if (Fin == "")
		      {
		         mensajeError = "Debe ingresar un valor para el campo 'Fecha Fin'.";
		      }
		      else if (!Fechas.validar(Ini))
		      {
		         mensajeError = "En el campo 'Fecha Inicio' debe ingresar un valor con el formato dd/mm/aaaa.";
		      }
		      else if (!Fechas.validar(Fin)){
		    	  mensajeError = "En el campo 'Fecha Fin' debe ingresar un valor con el formato dd/mm/aaaa.";
		      }
		      else if(I.after(F))
		      {
		    	  mensajeError = "La fecha de Inicio NO debe ser una fecha posterior a la fecha de Fin";
		      }

		      if (mensajeError != null)
		      {
		         JOptionPane.showMessageDialog(this, mensajeError, "Error", JOptionPane.ERROR_MESSAGE);
		         return false;
		      }
		      return true;
		   }
		 
		private void refrescarTabla(String SQL) {
		    try {
		        // seteamos la consulta a partir de la cual se obtendran los datos para llenar la tabla
		    	tabla.setSelectSql(SQL);
		    	// obtenemos el modelo de la tabla a partir de la consulta para 
		    	// modificar la forma en que se muestran de algunas columnas
		    	tabla.createColumnModelFromQuery();
		    	for (int i = 0; i < tabla.getColumnCount(); i++) { 
		    	    // para que muestre correctamente los valores de tipo TIME (hora)  		   		  
		    		if (tabla.getColumn(i).getType()==Types.TIME) {    		 
		    		    tabla.getColumn(i).setType(Types.CHAR);  
		  	       	}
		    		// cambiar el formato en que se muestran los valores de tipo DATE
		    		if (tabla.getColumn(i).getType()==Types.DATE) {
		    		    tabla.getColumn(i).setDateFormat("dd/MM/YYYY");
		    		}
		        }  
		    	// actualizamos el contenido de la tabla.
		    	tabla.refresh();
		    	// No es necesario establecer  una conexion, crear una sentencia y recuperar el 
		    	// resultado en un resultSet, esto lo hace automaticamente la tabla (DBTable) a 
		    	// patir de la conexion y la consulta seteadas con connectDatabase()
		    }
		    catch (SQLException ex) {
		        // en caso de error, se muestra la causa en la consola
		    	ex.printStackTrace();
		        System.out.println("SQLException: " + ex.getMessage());
		        System.out.println("SQLState: " + ex.getSQLState());
		        System.out.println("VendorError: " + ex.getErrorCode());
		        JOptionPane.showMessageDialog(SwingUtilities.getWindowAncestor(this),ex.getMessage() + "\n", "Error al ejecutar la consulta.",JOptionPane.ERROR_MESSAGE);
		    }
		    inicializarCampos();
		}
}