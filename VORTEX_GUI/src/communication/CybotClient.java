package communication;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import ui.components.MissionCommandGUI;

public class CybotClient extends Socket
{
    private PrintWriter cybot_out;
    private BufferedReader cybot_in;
    private Thread cybot_input_processing_thread;
    private MissionCommandGUI gui;

    /**
     * Cybot TCP Client connection
     * 
     * Starts a background thread for processing signals from the bot
     * @param ip IP address to connect to
     * @param port Port to connect to
     * @param gui Reference to GUI for relaying data
     * @throws IOException
     */
    public CybotClient(String ip, int port, MissionCommandGUI gui) throws IOException
    {
        super(ip, port);
        this.cybot_out = new PrintWriter(this.getOutputStream());
        this.cybot_in = new BufferedReader(new InputStreamReader(this.getInputStream()));
        this.cybot_input_processing_thread = new Thread(new CybotReader(cybot_in, gui));
        this.cybot_input_processing_thread.start();
        this.gui = gui;
    }

    /**
     * Sends the contents of the command text field to the robot
     */
    public void send_command()
    {
        if(cybot_out != null)
        {
            String command = gui.getCommandPanel().getCommand();
            gui.getCommandPanel().getCommandField().setText("");

            cybot_out.print(command);
            cybot_out.flush();
            gui.addToStatusOutput("Sent command to robot : " + command);
        }
        else
        {
            gui.addToStatusOutput("TCP connection not established with CyBot");
        }
    }

    /**
     * Puts the robot signal reading thread to sleep for the designated number of milliseconds
     * @param millis
     * @throws InterruptedException
     */
    public void sleep_cybot_input(long millis) throws InterruptedException
    {
        this.cybot_input_processing_thread.sleep(millis);
    }
}
