import javax.swing.*;
import javax.swing.event.*;
import java.awt.*;

import ui.components.*;

public class MissionCommandGUI extends JFrame
{
    public MissionCommandGUI()
    {
        setTitle("VORTEX Mission Command");
        setSize(900, 700);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
    }

    public void init_ui()
    {
        Container contentPane = this.getContentPane();
        contentPane.setLayout(new GridBagLayout());

        GridBagConstraints constraints = new GridBagConstraints();

        constraints.gridx = 0;
        constraints.gridy = 0;

        constraints.weightx = 1;
        constraints.weighty = 0.33;

        constraints.anchor = GridBagConstraints.WEST;

        constraints.fill = GridBagConstraints.BOTH;

        constraints.insets = new Insets(5, 5,5, 5);

        EnvironmentDisplay environment = new EnvironmentDisplay();
        environment.setPreferredSize(new Dimension(600, 400));

        contentPane.add(new EnvironmentDisplay(), constraints);

        constraints.gridx++;

        contentPane.add(new TCPPanel(), constraints);

        constraints.gridy++;

        contentPane.add(new SensorPanel(), constraints);

        constraints.gridx = 0;
        constraints.gridy++;

        constraints.anchor = GridBagConstraints.CENTER;
        constraints.fill = GridBagConstraints.HORIZONTAL;

        contentPane.add(new CommandPanel(), constraints);
    }


    public static void main(String[] args)
    {
        MissionCommandGUI vortex = new MissionCommandGUI();
        vortex.init_ui();
        vortex.setVisible(true);
    }
}
