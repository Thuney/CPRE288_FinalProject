import java.awt.event.ActionEvent;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

public class MissionCommand
{
	private MissionCommandGUI gui;
	private Socket tcp_socket = null;
	
	public MissionCommand()
	{
		gui = new MissionCommandGUI();
        gui.init_ui();
        gui.setVisible(true);
	}
	
	private void start_tcp(String ip, int port) throws UnknownHostException, IOException
    {
    	this.tcp_socket = new Socket(ip, port);
    }
	
	public void send_command()
    {
		
    }

    public void addToStatusOutput(String info)
    {
    	gui.getCommandPanel().getStatusArea().append(info);
    }
	
	public static void main(String[] args)
    {
		MissionCommand vortex = new MissionCommand();
    }
}
