#include <simplecpp>
#include<sstream>
#include "shooter.h"
#include "bubble.h"
//CS101 Project: Author-Soham Nivargi(Roll No. 21D070074

/* Simulation Vars */
const double STEP_TIME = 0.04;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);

//Probably the most imporatnt function is written first in this program
int intersect_bullet(vector<Bullet> &bullets,vector<Bubble> &bubbles,int x){
    //x is score
    for(unsigned int i=0;i<bubbles.size();i++){
        //bubble co-ordinates
        double x1=bubbles[i].get_center_x();
        double y1=bubbles[i].get_center_y();
        double r1=bubbles[i].get_radius();
        //bubble velocities
        double v1=bubbles[i].get_vx();
        double v2=bubbles[i].get_vy();
        Color c1=bubbles[i].get_color();
        for(unsigned int j=0;j<bullets.size();j++){
            //bullet co-ordinates
            double x2=bullets[j].get_center_x();
            double y2=bullets[j].get_center_y();
            double r2=bullets[j].get_height();r2=r2/2;r2=r2*sqrt(2);
            //The approx condition for bubble and bullet intersection
            if((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)<=(r1+r2)*(r1+r2)){
                bullets.erase(bullets.begin()+j);//Disappear the bullet which hit it
                x++;
                if(r1!=10){
                    //if on level 2 or level 3,split the bullet hit into two parts going in opposite direction
                    bubbles.push_back(Bubble(x1,y1,r1/2,1.5*v1,v2,c1));
                    bubbles.push_back(Bubble(x1,y1,r1/2,(-1.5)*v1,-v2,c1));
                }
                bubbles.erase(bubbles.begin()+i);//Disappear the bubble which got hit
                i--;//This is beacuse the vector shifts back
                break;//The bubble is gone now so move to next bubble
            }
        }
    }
    return x;
}


bool circle_itrsct(vector<Bubble> &bubbles,double x1,double y1,double r1){
    //To check if circle(head) of shooter intersects with bubble
    for(unsigned int i=0;i<bubbles.size();i++){
        double x2=bubbles[i].get_center_x();
        double y2=bubbles[i].get_center_y();
        double r2=bubbles[i].get_radius();
        if((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)<=(r1+r2)*(r1+r2)){
            bubbles[i].update_pos();//respawning of bubble which hit it
            return true;
            }
    }
    return false;
}

bool rect_itrsct(vector<Bubble> &bubbles,double x1,double y1,double w,double h){
    //To check if rectangle(body) of shooter intersects with bubble
    double r1=sqrt((w*w+h*h)/4);
    for(unsigned int i=0;i<bubbles.size();i++){
        double x2=bubbles[i].get_center_x();
        double y2=bubbles[i].get_center_y();
        double r2=bubbles[i].get_radius();
        if((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)<=(r1+r2)*(r1+r2)){
            bubbles[i].update_pos();//respawning of bubble which hit it
            return true;
        }
    }
    return false;
}

void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);//out of canvas
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

vector<Bubble> create_bubbles()
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 75, COLOR(255,105,180)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 75, COLOR(255,105,180)));
    return bubbles;
}

vector<Bubble> create_bubbles2()
{
    //create initial bubbles in level 2
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, 20, -200, 75, COLOR(105,180,255)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, 20, 200, 75, COLOR(105,180,255)));
    return bubbles;
}

vector<Bubble> create_bubbles3()
{
    //create initial bubbles in level 3
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, 40, 200, 75, COLOR(180,255,105)));
    bubbles.push_back(Bubble(WINDOW_X/3.0, BUBBLE_START_Y, 40, -200, 75, COLOR(180,255,105)));
    bubbles.push_back(Bubble(WINDOW_X/1.5, BUBBLE_START_Y, 40, 200, 75, COLOR(180,255,105)));
    return bubbles;
    }

int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);
    //The words displayed on screen
    Text show_health(330,BOTTOM_MARGIN, "HEALTH: ");
    Text for_time(150,BOTTOM_MARGIN,"TIME: ");
    Text time_limit(250,BOTTOM_MARGIN,"/ 20 ");
    Text Score(430,BOTTOM_MARGIN,"SCORE: ");
    //The live score will be displayed
    int p=0;
    string your_score;
    Text live_score(465,BOTTOM_MARGIN,"0");

    //Rectangles to avoid any confusion
    Rectangle r1(200,BOTTOM_MARGIN,160,20);
    Rectangle r2(340,BOTTOM_MARGIN,100,20);
    Rectangle r3(435,BOTTOM_MARGIN,80,20);

    //The time clock is displayed
    int i=0;
    Text current(200,BOTTOM_MARGIN,"0");
    string num;
//LEVEL1 LEVEL1 LEVEL1 LEVEL1 LEVEL1 LEVEL1 LEVEL1 LEVEL1 LEVEL1 LEVEL1 LEVEL1 LEVEL1 LEVEL1 LEVEL1 LEVEL1 LEVEL1
    Text level_1(250,250,"Level 1");
    wait(1);
    level_1.setMessage("");
    //Health for level 1 is 1
    int health1=1;
    string life;
    Text lives(370,BOTTOM_MARGIN,"1");

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles();

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;


    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }
        //shooter co-ordinates at each step_time
        double xrect=shooter.get_body_center_x();
        double yrect=shooter.get_body_center_y();
        double width=shooter.get_body_width();
        double height=shooter.get_body_height();

        double xcirc=shooter.get_head_center_x();
        double ycirc=shooter.get_head_center_y();
        double rcirc=shooter.get_head_radius();


        //update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);

        int q=intersect_bullet(bullets,bubbles,p);
        //if bullet and bubble intersect,score changes
        if(q!=p){
            p=q;
            stringstream scr;
            scr << p;
            scr >> your_score;
            live_score.setMessage(your_score);
        }
        //if bubble and shooter intersect health decreases
        if(circle_itrsct(bubbles,xcirc,ycirc,rcirc) || rect_itrsct(bubbles,xrect,yrect,width,height)){
            health1--;
            stringstream remaining;
            remaining << health1;
            remaining >> life;
            lives.setMessage(life);
        }

        i++;//this is for time change at every 50 Step_Times

        if(i%25==0){
            int j=i/25;
            stringstream time;
            time << j;
            time >> num;
            current.setMessage(num);
        }

        wait(STEP_TIME);

        if(bubbles.size()==0){
            //erase the remaining bubbles on screen and break this while loop to go to level 2
            for(unsigned int i=0;i<bullets.size();i++){
                bullets.erase(bullets.begin()+i);
                i--;
            }
            Text L1_done(250,250,"Well Done! Now,Level 2");
            wait(1);
            L1_done.setMessage("");
            break;
        }
        if(health1==0 || i==500){
            //Bad luck
            Text Game_Over(250,250,"Game Over");

            Text End_Score(240,270,"Final Score:");
            Text Game_end(290,270,p);
            wait(5);
            return 0;

        }

    }

//Level 2 Level 2 Level 2 Level 2 Level 2 Level 2 Level 2 Level 2 Level 2 Level 2 Level 2 Level 2 Level 2 Level 2 Level 2 Level 2 Level 2 Level 2


    bubbles = create_bubbles2();
    int i2=0;
    int health2=3;
    current.setMessage("0");
    lives.setMessage("3");
    time_limit.setMessage(" /30 ");
    //Health is now 3 and time limit increases to 30

     while(true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }
        //Shooter position
        double xrect=shooter.get_body_center_x();
        double yrect=shooter.get_body_center_y();
        double width=shooter.get_body_width();
        double height=shooter.get_body_height();

        double xcirc=shooter.get_head_center_x();
        double ycirc=shooter.get_head_center_y();
        double rcirc=shooter.get_head_radius();


        //update the bubbles
        move_bubbles(bubbles);

        //update the bullets
        move_bullets(bullets);

        int q=intersect_bullet(bullets,bubbles,p);
        //if bullet and bubble intersect, score increases
        if(q!=p){
            p=q;
            stringstream scr;
            scr << p;
            scr >> your_score;
            live_score.setMessage(your_score);
        }

        //if shooter and bubble intersect,health decreases
        if(circle_itrsct(bubbles,xcirc,ycirc,rcirc) || rect_itrsct(bubbles,xrect,yrect,width,height)){
            health2--;
            stringstream remaining;
            remaining << health2;
            remaining >> life;
            lives.setMessage(life);
        }

        i2++;//This is for time change

        if(i2%25==0){
            int j=i2/25;
            stringstream time;
            time << j;
            time >> num;
            current.setMessage(num);
        }

        wait(STEP_TIME);

        if(bubbles.size()==0){
            //erase other bullets on screen and break this while loop to go to level 3
            for(unsigned int i=0;i<bullets.size();i++){
                bullets.erase(bullets.begin()+i);
                i--;
            }
            Text L2_done(250,250,"Well Done! Now,Level 3");
            wait(1);
            L2_done.setMessage("");
            break;
        }
        if(health2==0 || i2==750){
            //Bad luck
            Text Game_Over(250,250,"Game Over");

            Text End_Score(240,270,"Final Score:");
            Text Game_end(290,270,p);
            wait(3);
            return 0;

        }
    }

//Level 3 Level 3 Level 3 Level 3 Level 3 Level 3 Level 3 Level 3 Level 3 Level 3 Level 3 Level 3 Level 3 Level 3 Level 3 Level 3 Level 3 Level 3
    bubbles=create_bubbles3();
    int i3=0;
    int health3=5;
    current.setMessage("0");
    lives.setMessage("5");
    time_limit.setMessage(" /50");
    //Health is provided as 5 as per level 3 difficulty

     while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }
        //update the bubbles
        move_bubbles(bubbles);

        //update the bullets
        move_bullets(bullets);

        //if bullet and bubble intersect, score increases
        int q=intersect_bullet(bullets,bubbles,p);
        if(q!=p){
            p=q;
            stringstream scr;
            scr << p;
            scr >> your_score;
            live_score.setMessage(your_score);
        }

        //Shooter position at every step
        double xrect=shooter.get_body_center_x();
        double yrect=shooter.get_body_center_y();
        double width=shooter.get_body_width();
        double height=shooter.get_body_height();

        double xcirc=shooter.get_head_center_x();
        double ycirc=shooter.get_head_center_y();
        double rcirc=shooter.get_head_radius();

        //If shooter and bubble intersect,health decreases
        if(circle_itrsct(bubbles,xcirc,ycirc,rcirc) || rect_itrsct(bubbles,xrect,yrect,width,height)){
            health3--;
            stringstream remaining;
            remaining << health3;
            remaining >> life;
            lives.setMessage(life);
        }

        i3++;//This is for time change

        if(i3%25==0){
            int j=i3/25;
            stringstream time;
            time << j;
            time >> num;
            current.setMessage(num);
        }

        wait(STEP_TIME);

        if(bubbles.size()==0){
            // End game with Thank you and erase the bullets
            for(unsigned int i=0;i<bullets.size();i++){
                bullets.erase(bullets.begin()+i);
                i--;
            }
            Text L3_done(250,250,"Well Done!Congratulations!!");
            Text End_Score(240,270,"Final Score:");
            Text Game_end(290,270,p);
            wait(3);
            L3_done.setMessage("");
            break;
        }
        if(health3==0 || i3==1250){
            //Bad luck
            Text Game_Over(250,250,"Game Over");

            Text End_Score(240,270,"Final Score:");
            Text Game_end(290,270,p);
            wait(3);
            return 0;

        }


    }
return 0;//exit canvas
}

























