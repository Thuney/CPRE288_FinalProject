package ui.components;

import javax.swing.*;
import java.awt.*;

public class SensorPanel extends JPanel
{

    private JTextField leftBump, rightBump, leftCliff, frontLeftCliff, frontRightCliff, rightCliff,
                       leftCliffSignal, frontLeftCliffSignal, frontRightCliffSignal, rightCliffSignal;

    public SensorPanel()
    {
        init_layout();
    }

    public void init_layout()
    {
        //Set panel layout manager as Grid Bag (https://docs.oracle.com/javase/tutorial/uiswing/layout/gridbag.html)
        setLayout(new GridBagLayout());
        GridBagConstraints constraints = new GridBagConstraints();

        //Left justify
        constraints.anchor = GridBagConstraints.WEST;

        //External padding
        constraints.insets = new Insets(5, 0, 5, 5);

        //Top left grid
        constraints.gridx = 0;
        constraints.gridy = 0;

        //Header Label
        constraints.gridx++;
        add(new JLabel("Sensor Data"), constraints);
        constraints.gridy++;

        //Bump detectors
        constraints.gridx = 0;
        add(new JLabel("Left Bump: "), constraints);
        constraints.gridx++;
        add(leftBump = new JTextField("-", 10), constraints);
        constraints.gridy++;

        constraints.gridx = 0;
        add(new JLabel("Right Bump: "), constraints);
        constraints.gridx++;
        add(rightBump = new JTextField("-", 10), constraints);
        constraints.gridy++;

        //Cliff Sensors
        constraints.gridx = 0;
        add(new JLabel("Left Cliff: "), constraints);
        constraints.gridx++;
        add(leftCliff = new JTextField("-", 10), constraints);
        constraints.gridy++;

        constraints.gridx = 0;
        add(new JLabel("Front Left Cliff: "), constraints);
        constraints.gridx++;
        add(frontLeftCliff = new JTextField("-", 10), constraints);
        constraints.gridy++;

        constraints.gridx = 0;
        add(new JLabel("Front Right Cliff: "), constraints);
        constraints.gridx++;
        add(frontRightCliff = new JTextField("-", 10), constraints);
        constraints.gridy++;

        constraints.gridx = 0;
        add(new JLabel("Right Cliff: "), constraints);
        constraints.gridx++;
        add(rightCliff = new JTextField("-", 10), constraints);
        constraints.gridy++;

        //Cliff Sensor Signals (?)
        constraints.gridx = 0;
        add(new JLabel("Left Cliff Signal: "), constraints);
        constraints.gridx++;
        add(leftCliffSignal = new JTextField("-", 10), constraints);
        constraints.gridy++;

        constraints.gridx = 0;
        add(new JLabel("Front Left Cliff Signal: "), constraints);
        constraints.gridx++;
        add(frontLeftCliffSignal = new JTextField("-", 10), constraints);
        constraints.gridy++;

        constraints.gridx = 0;
        add(new JLabel("Front Right Cliff Signal: "), constraints);
        constraints.gridx++;
        add(frontRightCliffSignal = new JTextField("-", 10), constraints);
        constraints.gridy++;

        constraints.gridx = 0;
        add(new JLabel("Right Cliff Signal: "), constraints);
        constraints.gridx++;
        add(rightCliffSignal = new JTextField("-", 10), constraints);
        constraints.gridy++;
    }

    public void updateLeftBump(String value)
    {
        leftBump.setText(value);
    }

    public void updateRightBump(String value)
    {
        rightBump.setText(value);
    }

    public void updateLeftCliff(String value)
    {
        leftCliff.setText(value);
    }

    public void updateFrontLeftCliff(String value)
    {
        frontLeftCliff.setText(value);
    }

    public void updateFrontRightCliff(String value)
    {
        frontRightCliff.setText(value);
    }

    public void updateRightCliff(String value)
    {
        rightCliff.setText(value);
    }

    public void updateLeftCliffSignal(String value)
    {
        leftCliffSignal.setText(value);
    }

    public void updateFrontLeftCliffSignal(String value)
    {
        frontLeftCliffSignal.setText(value);
    }

    public void updateFrontRightCliffSignal(String value)
    {
        frontRightCliffSignal.setText(value);
    }

    public void updateRightCliffSignal(String value)
    {
        rightCliffSignal.setText(value);
    }
}
