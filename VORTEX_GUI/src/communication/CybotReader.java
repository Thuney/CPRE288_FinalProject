package communication;

import data.Obstacle;
import ui.components.MissionCommandGUI;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;

public class CybotReader implements Runnable
{
    private MissionCommandGUI gui;
    private BufferedReader input_stream;

    /**
     * A runnable object that constantly reads available input from the CyBot
     * @param input_stream
     * @param gui
     */
    public CybotReader(BufferedReader input_stream, MissionCommandGUI gui)
    {
        this.input_stream = input_stream;
        this.gui = gui;
    }

    @Override
    public void run()
    {
        while(true)
        {
            try
            {
                if(this.input_stream.ready())
                {
                    handle_input();
                }
            }
            catch (IOException e)
            {
                e.printStackTrace();
            }
        }
    }

    /**
     * Handles the next line of input
     * @throws IOException
     */
    private void handle_input() throws IOException
    {
        String nextLine = input_stream.readLine();
        switch(nextLine)
        {
            case "Sweeping...":
            {
                gui.addToStatusOutput(nextLine);
                gui.getEnvironment().setObstacles(read_obstacle_data(input_stream));
                gui.getEnvironment().repaint();
                break;
            }
            default:
            {
            	gui.addToStatusOutput(nextLine);
            }
        }
    }

    /**
     * Creates new Obstacle objects for each detected object, then returns that array to the Environment display for drawing
     * @param input_stream Stream to collect obstacles from
     * @return
     * @throws IOException
     */
    private Obstacle[] read_obstacle_data(BufferedReader input_stream) throws IOException
    {
        ArrayList<Obstacle> obstacles = new ArrayList<Obstacle>(0);

        String input;
        while(true)
        {
            while(!input_stream.ready());
            input = input_stream.readLine();

            if(input.equals("..Finished")) break;

            //Regular expression to separate digits from the string
            String[] numbers = input.replaceAll("^\\D+","").split("\\D+");
            obstacles.add(new Obstacle(Integer.parseInt(numbers[0]), Integer.parseInt(numbers[1])));
        }

        Obstacle[] o = new Obstacle[obstacles.size()];
        obstacles.toArray(o);
        return o;
    }
}
