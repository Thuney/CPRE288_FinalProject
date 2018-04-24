package ui.components;

import javax.swing.*;
import java.awt.*;

public class TCPPanel extends JPanel
{
    private JTextField ip_address, port_number;
    private JButton connect_button;

    public TCPPanel()
    {
        init_layout();
    }

    private void init_layout()
    {
        //Set panel layout manager as Grid Bag (https://docs.oracle.com/javase/tutorial/uiswing/layout/gridbag.html)
        setLayout(new GridBagLayout());
        GridBagConstraints constraints = new GridBagConstraints();

        constraints.insets = new Insets(5,5 , 5, 5);

        //Top left grid
        constraints.gridx = 0;
        constraints.gridy = 0;

        //Top label
        constraints.gridx++;
        constraints.anchor = GridBagConstraints.CENTER;
        add(new JLabel("TCP Connection"), constraints);
        constraints.gridy++;

        //Left justify
        constraints.anchor = GridBagConstraints.WEST;

        //Reset x
        constraints.gridx = 0;

        //Add text field labels
        add(new JLabel("IP Address: "), constraints);
        constraints.gridy++;
        add(new JLabel("Port Number: "), constraints);

        //Reorient grid position
        constraints.gridx++;
        constraints.gridy = 1;
        constraints.weightx = 1;
        constraints.fill = GridBagConstraints.HORIZONTAL;

        //Add text fields, instantiating instance variables
        add((ip_address = new JTextField(10)), constraints);
        constraints.gridy++;
        add((port_number = new JTextField(10)), constraints);

        //Reorient
        constraints.gridy++;
        constraints.gridx = 1;

        //Create button to start TCP connection, binding it to an ActionListener
        connect_button = new JButton("Start TCP");
        add(connect_button, constraints);
    }

    public String get_ip()
    {
        return ip_address.getText();
    }

    public String get_port()
    {
        return port_number.getText();
    }
    
    public JButton getConnectButton()
    {
    	return this.connect_button;
    }
}
