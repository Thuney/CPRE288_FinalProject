package ui.components;

import javax.swing.*;
import java.awt.*;

public class CommandPanel extends JPanel
{
    private JTextField command_input;

    private JTextArea status_window;

    private JButton send_command_button;

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
        constraints.gridy = 0;

        //Left align
        constraints.anchor = GridBagConstraints.WEST;

        //Command text field
        add(new JLabel("Commands: "), constraints);
        constraints.gridx++;
        add(command_input = new JTextField(20), constraints);
        constraints.gridx++;

        //Send command button
        send_command_button = new JButton("Send command");
        add(send_command_button, constraints);

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

        status_window = new JTextArea(8, 30);
        JScrollPane scroll_pane = new JScrollPane(status_window);
        status_window.setBorder(BorderFactory.createLineBorder(new Color(0,0,0)));

        add(scroll_pane, constraints);
    }
    
    public String getCommand()
    {
    	return this.command_input.getText();
    }
    
    public JTextField getCommandField()
    {
        return this.command_input;
    }

    public JTextArea getStatusArea()
    {
    	return this.status_window;
    }

    public JButton getSendCommandButton()
    {
        return send_command_button;
    }
}
