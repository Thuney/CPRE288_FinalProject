package ui.components;


import javax.swing.*;
import java.awt.*;

import ui.components.*;

public class MissionCommandGUI extends JFrame
{
	private EnvironmentDisplay environment;
	private TCPPanel tcp;
	private SensorPanel sensor_info;
	private CommandPanel command_panel;
	
    public MissionCommandGUI()
    {
        setTitle("VORTEX Mission Command");
        setSize(900, 700);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
    }

    public void init_ui()
    {
    	/*
    	 * UI Components
    	 */
    	environment = new EnvironmentDisplay();
    	tcp = new TCPPanel();
    	sensor_info = new SensorPanel();
    	command_panel = new CommandPanel();
    	
        Container contentPane = this.getContentPane();
        contentPane.setLayout(new GridBagLayout());

        GridBagConstraints constraints = new GridBagConstraints();

        constraints.gridx = 0;  
        constraints.gridy = 0;

        //Margins
        constraints.insets = new Insets(5, 5, 5, 5);

        //Left align components
        constraints.anchor = GridBagConstraints.WEST;
        constraints.fill = GridBagConstraints.BOTH;

        /*
         * Environment Display
         */
        
        constraints.weightx = 3;
        constraints.weighty = 0.5;
        
        constraints.gridwidth = 2;
        constraints.gridheight = 2;

        environment.setBorder(BorderFactory.createLineBorder(new Color(0,0,0)));

        contentPane.add(environment, constraints);
        
        /*
         * TCP Connection Panel
         */
        constraints.gridx = 2;
        
        constraints.weightx = 0.5;
        constraints.weighty = 0.5;
        
        constraints.gridwidth = GridBagConstraints.REMAINDER;
        constraints.gridheight = 1;
        
        contentPane.add(tcp, constraints);
        
        /*
         * Sensor Information Panel
         */
        constraints.gridy = 1;
    	constraints.gridx = 2;
        
        constraints.weightx = 0.5;
        constraints.weighty = 0.5;
        
        constraints.gridwidth = GridBagConstraints.REMAINDER;
        constraints.gridheight = 1;
        
        contentPane.add(sensor_info, constraints);
        
        /*
         * Command and Status Panel
         */
        constraints.gridy = 2;
    	constraints.gridx = 0;
        
        constraints.weightx = 0.5;
        constraints.weighty = 0.5;
        
        constraints.gridwidth = 3;
        constraints.gridheight = 1;
        
        contentPane.add(command_panel, constraints);        
    }
    
    public EnvironmentDisplay getEnvironment()
    {
    	return this.environment;
    }
    
    public TCPPanel getTCPPanel()
    {
    	return this.tcp;
    }
    
    public SensorPanel getSensorPanel()
    {
    	return this.sensor_info;
    }
    
    public CommandPanel getCommandPanel()
    {
    	return this.command_panel;
    }

    public void addToStatusOutput(String info)
    {
        command_panel.getStatusArea().append("\n" + info);
    }
}
