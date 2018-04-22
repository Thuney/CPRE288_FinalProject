import data.Obstacle;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Random;

public class MissionCommand
{
	private MissionCommandGUI gui;
	private Socket tcp_socket = null;
	private BufferedReader cybot_in;
	private PrintWriter cybot_out;
	
	public MissionCommand()
	{
		gui = new MissionCommandGUI();
        gui.init_ui();
        gui.setVisible(true);

        gui.getTCPPanel().getConnectButton().addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                String ip = gui.getTCPPanel().get_ip();
                int port_number = Integer.parseInt(gui.getTCPPanel().get_port());
                try
                {
                    start_tcp(ip, port_number);
                }
                catch (IOException e1)
                {
                    addToStatusOutput("Could not connect to " + ip + " : " + port_number);
                    e1.printStackTrace();
                }
            }
        });

        gui.getCommandPanel().getSendCommandButton().addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                send_command();
            }
        });
	}
	
	private void start_tcp(String ip, int port) throws UnknownHostException, IOException
    {
    	this.tcp_socket = new Socket(ip, port);
    	if(tcp_socket.isConnected())
    	{
    		cybot_out = new PrintWriter(tcp_socket.getOutputStream());
    		cybot_in = new BufferedReader(new InputStreamReader(tcp_socket.getInputStream()));
    		addToStatusOutput("TCP Connection Established with CyBot");
    	}
    }
	
	public void send_command()
    {
		String command = gui.getCommandPanel().getCommand();
        gui.getCommandPanel().getCommandField().setText("");
        if(cybot_out != null)
        {
        	cybot_out.print(command);
        	cybot_out.flush();
//        	cybot_out.print("\r");
        	addToStatusOutput("Sent command to robot : " + command);
        }
        else
        {
        	addToStatusOutput("TCP connection not established with CyBot");
        }
    }

    public void addToStatusOutput(String info)
    {
    	gui.getCommandPanel().getStatusArea().append("\n" + info);
    }
	
	public static void main(String[] args)
    {
        MissionCommand vortex = new MissionCommand();

        Obstacle[] obstacles = new Obstacle[9];

        Random r = new Random();

        obstacles[0] = new Obstacle(10, r.nextInt(300) + 100);
        obstacles[1] = new Obstacle(30, r.nextInt(300) + 100);
        obstacles[2] = new Obstacle(50, r.nextInt(300) + 100);
        obstacles[3] = new Obstacle(70, r.nextInt(300) + 100);
        obstacles[4] = new Obstacle(90, r.nextInt(300) + 100);
        obstacles[5] = new Obstacle(110, r.nextInt(300) + 100);
        obstacles[6] = new Obstacle(130, r.nextInt(300) + 100);
        obstacles[7] = new Obstacle(150, r.nextInt(300) + 100);
        obstacles[8] = new Obstacle(170, r.nextInt(300) + 100);

        vortex.gui.getEnvironment().setObstacles(obstacles);

        vortex.gui.revalidate();
    }
}
