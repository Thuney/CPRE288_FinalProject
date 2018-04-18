package ui.components;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class CommandPanel extends JPanel
{
    private JTextField command_input;

    private JTextArea status_window;

    public CommandPanel()
    {
        init_layout();
    }

    private void init_layout()
    {
        //Set panel layout manager as Grid Bag (https://docs.oracle.com/javase/tutorial/uiswing/layout/gridbag.html)
        setLayout(new GridBagLayout());
        GridBagConstraints constraints = new GridBagConstraints();

        //External padding
        constraints.insets = new Insets(5,5 , 5, 5);

        //Top left grid
        constraints.gridx = 0;
        constraints.gridy = 1;

        //Left align
        constraints.anchor = GridBagConstraints.WEST;

        //Command text field
        add(new JLabel("Commands: "), constraints);
        constraints.gridx++;
        add(command_input = new JTextField(20), constraints);
        constraints.gridx++;

        //Send command button
        JButton sendCommand = new JButton("Send command");
        add(sendCommand, constraints);

        //Orient
        constraints.gridy = 0;
        constraints.gridx++;

        //Empty space padding
        add(Box.createHorizontalStrut(50), constraints);

        constraints.gridx++;
        constraints.anchor = GridBagConstraints.CENTER;

        //Status window
        add(new JLabel("Status Window"), constraints);
        constraints.gridy++;

        status_window = new JTextArea(8, 20);
        status_window.setBorder(BorderFactory.createLineBorder(new Color(0,0,0)));

        add(status_window, constraints);
    }
    
    public String getCommand()
    {
    	return this.command_input.getText();
    }
    
    public JTextArea getStatusArea()
    {
    	return this.status_window;
    }
}
