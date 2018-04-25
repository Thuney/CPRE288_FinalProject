

import communication.CybotClient;
import data.Obstacle;
import ui.components.MissionCommandGUI;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.net.UnknownHostException;
import java.util.Random;

public class MissionCommand
{
	private MissionCommandGUI gui;
	private CybotClient cybot_client;
	
	public MissionCommand()
	{
		//Construction of GUI
		gui = new MissionCommandGUI();
        gui.init_ui();
        gui.setVisible(true);

        //Listener for TCP connection button event
        gui.getTCPPanel().getConnectButton().addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                String ip = gui.getTCPPanel().get_ip();
                int port_number = Integer.parseInt(gui.getTCPPanel().get_port());
                try
                {
                    open_tcp_client_connection(ip, port_number);
                }
                catch (IOException e1)
                {
                    gui.addToStatusOutput("Could not connect to " + ip + " : " + port_number);
                    e1.printStackTrace();
                }
            }
        });

        //Listener for 'Send Command' button event
        gui.getCommandPanel().getSendCommandButton().addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                cybot_client.send_command();
            }
        });
	}
	
	/*
	 * Constructs a CybotClient object, which acts as a TCP connection to the robot UART
	 */
	private void open_tcp_client_connection(String ip, int port) throws UnknownHostException, IOException
    {
    	this.cybot_client = new CybotClient(ip, port, gui);
    	if(cybot_client.isConnected())
    	{
    		//Print to status output, indicating connected status
    		gui.addToStatusOutput("TCP Connection Established with CyBot");
    	}
    }

    public static void main(String[] args)
    {
        MissionCommand vortex = new MissionCommand();

//        Obstacle[] obstacles = new Obstacle[9];
//
//        Random r = new Random();
//
//        obstacles[0] = new Obstacle(10, r.nextInt(20) + 30);
//        obstacles[1] = new Obstacle(30, r.nextInt(20) + 30);
//        obstacles[2] = new Obstacle(50, r.nextInt(20) + 30);
//        obstacles[3] = new Obstacle(70, r.nextInt(20) + 30);
//        obstacles[4] = new Obstacle(90, r.nextInt(20) + 30);
//        obstacles[5] = new Obstacle(110, r.nextInt(20) + 30);
//        obstacles[6] = new Obstacle(130, r.nextInt(20) + 30);
//        obstacles[7] = new Obstacle(150, r.nextInt(20) + 30);
//        obstacles[8] = new Obstacle(170, r.nextInt(20) + 30);
//
//        vortex.gui.getEnvironment().setObstacles(obstacles);
//
//        vortex.gui.revalidate();
    }
}
