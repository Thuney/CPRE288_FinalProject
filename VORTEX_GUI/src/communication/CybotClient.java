package communication;

import vortex.MissionCommandGUI;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class CybotClient extends Socket
{
    private PrintWriter cybot_out;
    private BufferedReader cybot_in;
    private Thread cybot_input_processing_thread;
    private MissionCommandGUI gui;

    public CybotClient(String ip, int port, MissionCommandGUI gui) throws IOException
    {
        super(ip, port);
        this.cybot_out = new PrintWriter(this.getOutputStream());
        this.cybot_in = new BufferedReader(new InputStreamReader(this.getInputStream()));
        this.cybot_input_processing_thread = new Thread(new CybotReader(cybot_in, gui));
        this.cybot_input_processing_thread.start();
        this.gui = gui;
    }

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

    public void sleep_cybot_input(long millis) throws InterruptedException
    {
        this.cybot_input_processing_thread.sleep(millis);
    }
}
