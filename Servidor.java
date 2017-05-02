import java.io.PrintWriter;
import java.io.FileWriter;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.EOFException;
import java.io.IOException;

import java.net.ServerSocket;
import java.net.Socket;
import java.net.BindException;

import java.util.logging.Level;
import java.util.logging.Logger;

public class Servidor extends Thread{
    private PrintWriter writer; // output stream to client
    private BufferedReader reader; // input stream from client
    private ServerSocket server; // server socket
    private Socket conexao; // connection to client
    private int port;
    
    @Override
    public void run() {
        try // set up server to receive connections; process connections
        {
            if(server != null)
                server.close();
            port = 1992;
            while(server == null || server.getLocalPort() == -1){
                try{
                    server = new ServerSocket(port); // create ServerSocket     
                    server.setReuseAddress(true);
                } catch (BindException e){ //caso a porta ja esteja em uso.
                    port++;
                }
            }
            System.out.println("Servidor conectado pela porta " + port);
            PrintWriter p = new PrintWriter(new FileWriter("port.txt"), true);
            //PrintWriter p2 = new PrintWriter(new FileWriter("../port.txt"), true);
            p.print(port);
            p.close();
            //p2.print(port);
            //p2.close();
            try {
                esperarConexao(); // wait for a connection
                iniciarStreams(); // get input & output streams
                receber(); // process connection
            } // end try
            catch(EOFException ex){
                System.out.println("Conexão com o cliente encerrada");
            } // end catch
            finally {
                fecharConexao(); //  close connection
            } // end finally
        } // end try
        catch(IOException ex){
            Logger.getLogger(Servidor.class.getName()).log(Level.SEVERE, null, ex);
        } // end catch
    } // end method run
    private void esperarConexao() throws IOException {
        System.out.println("Esperando Conexão");
        conexao = server.accept(); // allow server to accept connection       
        //conectou = true;
        System.out.println("Conexão recebida de: "
                + conexao.getInetAddress().getHostName());
    }
    private void iniciarStreams() throws IOException {
        writer = new PrintWriter(conexao.getOutputStream());
        writer.flush(); // flush output buffer to send header information
        // set up input stream for objects
        reader = new BufferedReader(new InputStreamReader(conexao.getInputStream()));
        System.out.println("Streams inicializados");
        this.enviar("Streams inicializados");
    }
    private void receber() throws IOException {
        String message = "";
        do // process messages sent from client
        {
            try {
                if(message != null){
                    message = reader.readLine();
                    System.out.print("RECEBIDO >> " + message); // display message
                    //if(message.split("/").length == 2) {
                    //    Main.addRank(message);
                    //}
                }
                message="";
            } catch(Exception e){
                this.fecharConexao();
            }
        } while(!conexao.isClosed());
    }
    public void fecharConexao() {
        System.out.println("Conexão do servidor encerrada");
        try {
            if(writer != null) {
                writer.close(); // close output stream
            }
            if(reader != null) {
                reader.close(); // close input stream
            }
            if(conexao != null) {
                conexao.close(); // close socket
            }
        } // end try
        catch(IOException ex){
            Logger.getLogger(Servidor.class.getName()).log(Level.SEVERE, null, ex);
        } // end catch
    }
    // send message to client
    public void enviar(String message) {
        writer.println(message);
        System.out.println("ENVIADO >> " + message);
    }

    public static void main (String [] args){
        Servidor serv=new Servidor();
        serv.start();
    }
}
