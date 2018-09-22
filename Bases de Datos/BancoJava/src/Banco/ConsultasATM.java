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
import javax.swing.SwingConstants;
import javax.swing.WindowConstants;
import javax.swing.text.MaskFormatter;

import EjemploClase.Fechas;
import quick.dbtable.DBTable;

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
	   private DBTable tabla; 
	   protected String NroTarjeta;
	   
	   protected Connection conexionBD = null;
	   
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
	                  lblNombre.setText("Nombre");
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
	                  lblFecha.setText("Fecha");
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
	   
 private void thisComponentShown(ComponentEvent evt) 
	   {
	      this.conectarBD();
	      this.refrescar();
	   }
	   private void btnUltimosMoviminetosActionPerformed(ActionEvent evt) {
			//Mostrar las ultimas 15 transacciones
		   try {
			   Statement stm = this.conexionBD.createStatement();
			   String sql = "SELECT  "
			   		+ "FROM  "
			   		+ "WHERE ";
			   stm.execute(sql);
			   stm.close();
		   }
		   catch(SQLException ex) {
			   JOptionPane.showMessageDialog(this,
                       "Se produjo un error al mostrar los ultimos movimientos.\n" + ex.getMessage(),
                       "Error",
                       JOptionPane.ERROR_MESSAGE);
		   }
		}
	   
	   
	   private void btnSaldoActionPerformed(ActionEvent evt) {
			//Mostrar el Saldo del Usuario, ver como hacer la consulta
		   	//ya que el saldo no es directamente accesible
		   try {
			   Statement stmt = this.conexionBD.createStatement();
		         String sql = "SELECT " +
		                      "FROM "
		                      + "WHERE " ;
		         stmt.execute(sql);
		         stmt.close();
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
				   //Revisar Query, se me confunde un toque con tanta información en el View
				   Statement stmt = this.conexionBD.createStatement();
			         String sql = "SELECT Fecha,hora,tipo,monto,cod_caja,destino" +
			                      "FROM trans_cajas_ahorro"
			                      + "WHERE Fecha >= " + txtFechaInicio.getText().trim() + "and Fecha <=" + txtFechaFin.getText().trim() ;
			         stmt.execute(sql);
			         stmt.close();
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
	      this.desconectarBD();
	   }
	   
	   private void conectarBD() {
		      if (this.conexionBD == null)
		      {
		        
		    	  try
		         {
		            Class.forName("com.mysql.cj.jdbc.Driver").newInstance();
		         }
		         catch (Exception ex)
		         {
		            System.out.println(ex.getMessage());
		         }
		        
		         try
		         {
		        	//Ver Esto
		            String servidor = "localhost:3306";
		            String baseDatos = "banco";
		            String usuario = "admin_batallas";
		            String clave = "pwadmin";
		            String uriConexion = "jdbc:mysql://" + servidor + "/" + baseDatos+"?serverTimezone=UTC";
		   
		            this.conexionBD = DriverManager.getConnection(uriConexion, usuario, clave);
		         }
		         catch (SQLException ex)
		         {
		            JOptionPane.showMessageDialog(this,
		                                          "Se produjo un error al intentar conectarse a la base de datos.\n" + ex.getMessage(),
		                                          "Error",
		                                          JOptionPane.ERROR_MESSAGE);
		            System.out.println("SQLException: " + ex.getMessage());
		            System.out.println("SQLState: " + ex.getSQLState());
		            System.out.println("VendorError: " + ex.getErrorCode());
		         }
		      }
		   }

		   private void desconectarBD()
		   {
		      if (this.conexionBD != null)
		      {
		         try
		         {
		            this.conexionBD.close();
		            this.conexionBD = null;
		         }
		         catch (SQLException ex)
		         {
		            System.out.println("SQLException: " + ex.getMessage());
		            System.out.println("SQLState: " + ex.getSQLState());
		            System.out.println("VendorError: " + ex.getErrorCode());
		         }
		      }
		   }
		   
		 //Revisar, no me parece necesario todo el código que este método contiene  
		 private void refrescar()
		   {
		      try
		      {
		         Statement stmt = this.conexionBD.createStatement();

		         String sql = "SELECT *" + 
		                      "FROM trans_cajas_ahorro " +
		                      "ORDER BY nombre_batalla";


		         ResultSet rs = stmt.executeQuery(sql);
		       
		                 
		        //actualiza el contenido de la tabla con los datos del result set rs
		         tabla.refresh(rs);

		        //setea el formato de visualizacion de la columna "fecha" a dia/mes/ano
		         tabla.getColumnByDatabaseName("fecha").setDateFormat("dd/MM/YYYY");
		         
		         tabla.getColumnByDatabaseName("fecha").setMinWidth(80);       
		         rs.close();
		         stmt.close();
		      }
		      catch (SQLException ex)
		      {
		         System.out.println("SQLException: " + ex.getMessage());
		         System.out.println("SQLState: " + ex.getSQLState());
		         System.out.println("VendorError: " + ex.getErrorCode());
		      }
		      
		      this.inicializarCampos();
		   }
		 
		 private void inicializarCampos()
		   {
		      this.txtFechaFin.setText("");
		      this.txtFechaInicio.setText("");
		   }
		 
		 //Esta función es necesaria para validar el input ANTES de realizar los query
		 private boolean validarCampos()
		   {
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
