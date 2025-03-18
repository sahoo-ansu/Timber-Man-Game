#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <math.h>
#include <sstream>
using namespace sf;

void updateCloud(Sprite &s, Time &dt, float &pixelPerSecond,bool gameOver,bool timeOut)
{
    static float x1=s.getPosition().x;
    static float y1=s.getPosition().y;
    float x = s.getPosition().x + dt.asSeconds() * pixelPerSecond;
    float y = s.getPosition().y;
    if (x > 1900)
    {
        s.setPosition(-200, y);
    }else if(gameOver || timeOut){
        s.setPosition(x1,y1);
    }
    else
    {
        s.setPosition(x, y);
    }
}

void updateBee(Sprite &s, Time &dt, float &pixelPerSecond,bool gameOver,bool timeOut)
{
    static float x1=s.getPosition().x;
    static float y1=s.getPosition().y;
    static float timex = 0.0f;
    timex += dt.asSeconds();
    float x = s.getPosition().x - dt.asSeconds() * pixelPerSecond;
    float y = 600 + 30 * sin(timex * 1);
    if (x < -200)
    {
        s.setPosition(2000, y);
    }else if(gameOver || timeOut){
        s.setPosition(x1,y1);
    }
    else
    {
        s.setPosition(x, y);
    }
}

void updateTimeBar(RectangleShape &timeBar,float &timeOutTime,float &timeBarPixPerSec,float &timeBarHeight,bool &timeOut,Time &dt){
    timeOutTime = timeOutTime - dt.asSeconds();
            float newTimeBarWidth = timeBarPixPerSec * timeOutTime;
            timeBar.setSize(Vector2f(newTimeBarWidth, timeBarHeight));
            if (newTimeBarWidth <= 0)
            {
                timeOut = true;
            }
            if (!timeOut)
            {
                timeBar.setSize(Vector2f(newTimeBarWidth, timeBarHeight));
            }
}

void updateBranch(Sprite *spriteBranch,int &chopheight,int &maxBranch){

    for(int i=maxBranch-1;i>0;i--){
        float x=spriteBranch[i-1].getPosition().x;
        float y=spriteBranch[i-1].getPosition().y;
        if(y>900){
            y=-160;
        }else{
            y=y+chopheight;
        }
        spriteBranch[i].setPosition(x,y);
        spriteBranch[i].setRotation(spriteBranch[i-1].getRotation());      
    }   

    float x=spriteBranch[0].getPosition().x;   
    int choice=rand()%3;

    if(choice==0){
        x=1330;
        spriteBranch[0].setRotation(0);

    }
    else if(choice==1){
        x=630;
        spriteBranch[0].setRotation(180);
    }
    else{
        x=3000;
    }

    spriteBranch[0].setPosition(x,-160);

}


int main()
{
    VideoMode vm(800, 600);
    RenderWindow window(vm, "Timber man!!!");

    View vw(FloatRect(0,0,1980,1080));
    window.setView(vw);
    Sprite spritebg;
    Texture texturebg;
    texturebg.loadFromFile("./background.png");
    spritebg.setTexture(texturebg);

    Sprite spriteCloud;
    Texture textureCloud;
    textureCloud.loadFromFile("cloud.png");
    spriteCloud.setTexture(textureCloud);
    spriteCloud.setPosition(0, 100);
    //spriteCloud.setScale(0.5, 0.5);
    bool moveCloud = false;
    float cloudSpeed = 12;
    float pixelPerSecond = 1160 / cloudSpeed;

    Sprite spriteCloud2;
    window.draw(spriteCloud2);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud2.setPosition(-300, 50);
    spriteCloud2.setScale(0.7, 0.7);
    float cloudSpeed2 = 18;
    float pixelPerSecond2 = 1200 / cloudSpeed2;

    Sprite spriteCloud3;
    spriteCloud3.setTexture(textureCloud);
    spriteCloud3.setPosition(40, 10);
    spriteCloud3.setScale(0.4, 0.4);
    float cloudSpeed3 = 25;
    float pixelPerSecond3 = 1160 / cloudSpeed3;

    Sprite spriteBee;
    Texture textureBee;
    textureBee.loadFromFile("bee.png");
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(1600, 600);
    //spriteBee.setScale(0.4, 0.4);
    bool moveBee = false;
    float beeSpeed = 30;
    float pixelPerSecondBee = 1100 / beeSpeed;

    Sprite spriteTree;
    Texture textureTree;
    textureTree.loadFromFile("tree2.png");
    spriteTree.setTexture(textureTree);
    spriteTree.setScale(1.4,1.4);
    spriteTree.setPosition(1920/2-150, 0);

    int score_val = 0;
    String score = "Score = 0";

    Text scoreMsg;
    Font font;
    font.loadFromFile("KOMIKAP_.ttf");
    scoreMsg.setFont(font);
    scoreMsg.setFillColor(Color::White);
    scoreMsg.setString(score);
    scoreMsg.setCharacterSize(50);
    scoreMsg.setPosition(100, 10);
    bool acceptInput = true;

    Text messageText;
    messageText.setFont(font);
    messageText.setFillColor(Color::White);
    messageText.setString("Please Enter to Start The Game!");
    messageText.setCharacterSize(50);

    messageText.setPosition(1920 / 2.0f, 1080 / 3.0f);

    bool paused = true;

    bool timeOut = false;
    RectangleShape timeBar;
    float timeBarWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setOrigin(400 / 2, 80 / 2);
    timeBar.setPosition(1920 / 2, 880);

    float timeOutTime = 6;
    float timeBarPixPerSec = timeBarWidth / timeOutTime;

    Sprite spriteBranch[6];
    Texture textureBranch;
    textureBranch.loadFromFile("branch.png");
    

    int maxBranch=6;
    for(int i=0;i<maxBranch;i++){
        spriteBranch[i].setTexture(textureBranch);
        spriteBranch[i].setPosition(3000,-260);
        spriteBranch[i].setOrigin(220,40);
        spriteBranch[i].setRotation(180);
    }

    int chopheight=200;

    Sprite spritePlayer;
    Texture texturePlayer;
    texturePlayer.loadFromFile("player.png");
    spritePlayer.setTexture(texturePlayer);
    
    FloatRect bounds=spritePlayer.getLocalBounds();
    spritePlayer.setOrigin(bounds.left+bounds.width/2.0f,bounds.top+bounds.height/2.0f);
    spritePlayer.setPosition(1310,900-bounds.height/2.0f);

    bool gameOver=false;

    Sprite spriteLog;
    Texture textureLog;
    textureLog.loadFromFile("log.png");
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810,780);

    float logSpeedX=0.1;
    float logPixelPerSecX=990/logSpeedX;
    float logSpeedY=0.1;
    float logPixelPerSecY=-400/logSpeedY;
    bool logActive=false;
    float logPixelPerX;



    Clock ct;
    Time dt;

    while (window.isOpen())
    {
        // close window
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        if (event.type == Event::KeyReleased && acceptInput == false)
        {
            acceptInput = true;
        } window.draw(messageText);

        dt = ct.restart();
        // move element
        if (!paused || logActive)
        {

            updateCloud(spriteCloud, dt, pixelPerSecond,gameOver,timeOut);
            updateCloud(spriteCloud2, dt, pixelPerSecond2,gameOver,timeOut);
            updateCloud(spriteCloud3, dt, pixelPerSecond3,gameOver,timeOut);

            updateBee(spriteBee, dt, pixelPerSecondBee,gameOver,timeOut);

            updateTimeBar(timeBar,timeOutTime,timeBarPixPerSec,timeBarHeight,timeOut,dt);
            
        }
        if(timeOut)
        {
            paused = true;
            messageText.setString("Time Out!!!!");
        }

    
        if (acceptInput)
        {
            if (Keyboard::isKeyPressed(Keyboard::Return))
            {
                moveCloud = true;
                moveBee = true;
                paused = !paused;
                if (paused && !logActive)
                {
                    messageText.setString("Paused!!!!");
                }
                else
                {
                    messageText.setString("");
                }
                if(timeOut){
                    timeOutTime=6;
                    timeOut=false;
                    score_val=0;
                }
                if(gameOver){
                    timeOutTime=6;
                    timeOut=false;
                    score_val=0;
                    gameOver=false;
                    paused=false;
                    for(int i=0;i<maxBranch;i++){
                        spriteBranch[i].setPosition(3000,-260);
                    }
                    
                    
                }
                
                acceptInput = false;
            }
            if (Keyboard::isKeyPressed(Keyboard::Right) && !paused && !gameOver)
            {
                score_val++;
                timeOutTime=6;
                updateBranch(spriteBranch,chopheight,maxBranch);


                float y=spritePlayer.getPosition().y;
                spritePlayer.setPosition(810+300+bounds.width/2.0f-10,y);
                spritePlayer.setScale(1.0f,1.0f);

                logPixelPerX=-logPixelPerSecX;
                logActive=true;

                acceptInput = false;
            }

            if (Keyboard::isKeyPressed(Keyboard::Left) && !paused && !gameOver)
            {
                score_val++;
                timeOutTime=6;
                updateBranch(spriteBranch,chopheight,maxBranch);

                float y=spritePlayer.getPosition().y;
                spritePlayer.setPosition(810-bounds.width/2.0f-10,y);
                spritePlayer.setScale(-1.0f,1.0f);

                logPixelPerX=logPixelPerSecX;
                logActive=true;

                acceptInput = false;
            }
        }

        FloatRect playerBound=spritePlayer.getGlobalBounds();
        FloatRect branchBound=spriteBranch[5].getGlobalBounds();

        if(playerBound.intersects(branchBound)){
            timeBar.setSize(Vector2f(0,0));
            messageText.setString("Game Over !!!!");
            paused=true;
            
            gameOver=true;
            
        }

        std::stringstream ss;
        ss << "Score = " << score_val;
        scoreMsg.setString(ss.str());

        FloatRect bounds = messageText.getLocalBounds();
        float mid_x = bounds.left + bounds.width / 2;
        float mid_y = bounds.top + bounds.height / 2;
        messageText.setOrigin(mid_x, mid_y);

        if(logActive){
            float x=spriteLog.getPosition().x;
            float y=spriteLog.getPosition().y;

            x=x+logPixelPerX*dt.asSeconds();
            y=y+logPixelPerSecY*dt.asSeconds();

            if(x<0 || x>1980){
                x=810;
                y=780;
                logActive=false;
            }
            spriteLog.setPosition(x,y);
        }

        window.clear();

        window.draw(spritebg);
        window.draw(spriteCloud3);
        window.draw(spriteCloud2);
        window.draw(spriteCloud);

        window.draw(spriteTree);
        for(int i=0;i<maxBranch;i++)
            window.draw(spriteBranch[i]);
        
        window.draw(spriteLog);    
        
        window.draw(spritePlayer);    
        
        window.draw(spriteBee);

        window.draw(scoreMsg);
        window.draw(messageText);
        window.draw(timeBar);
        window.display();
    }
    return 0;
}
