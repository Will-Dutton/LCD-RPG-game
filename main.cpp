#include <cstdint>
#include <mbed.h>
#include "TextLCD.h"
#include <cstdlib> //         
#include <ctime>
Thread button1Thread;
Thread stickBackThread;
Thread purrThread;
Thread timerThread;
Thread randomNoThread;
Semaphore button1Semaphore;
Semaphore continueSemaphore;
Semaphore stickBackSemaphore;
Semaphore timerStartSemaphore;
AnalogIn ainX(A0); 
AnalogIn ainY(A1);
AnalogIn ainZ(A2);
AnalogIn ainA(A3);
DigitalOut green(D6);
DigitalOut yellow(D7);
DigitalOut red(D8);
InterruptIn button1(BUTTON1);
int buff = 0;
int randomNo = 0;
bool potion = false;
bool lostHealth = false;
bool greenOn = false;
bool yellowOn = false;
bool redOn = false;
int lightColour;
int SFXBang = 0;
int SFXnumber1 = 0;
int SFXnumber2 = 0;
int SFXnumber3 = 0;
int musicBang = 0;
int musicNo =0;
int freq = 0;
int score ;
int waveCount = 0;
bool fighting = false;
bool goingUp = 0;
bool start =0;
bool timerStop = false;
bool timerPause = false;
int bangWin = 0;
int bangLose = 0;
float timerVal = 0;
bool button1Down = false;
int resetOscBang = 0;
bool audioOn = 0;   //more inputs for PD
float difficulty = 0;
TextLCD lcd(D0, D1, D2, D3, D4, D5, TextLCD::LCD20x4);
int gridHeight = 6;
int gridWidth = 7;
int choiceData[4] = {0,0,0,0};
bool visited[6][7] = {{0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0}};
bool forceFight[6][7] = {{0,0,0,0,0,0,0},
                    {0,1,0,0,1,0,0},
                    {0,0,0,0,1,0,0},
                    {0,0,0,1,0,0,0},
                    {0,0,0,0,0,0,0},
                    {0,0,0,0,0,1,0}};
int  diGrid[6][7] = {{0,24,25,27,26,19,0},
                    {0,23,21,22,20,18,0},
                    {17,16,15,28,12,13,14},
                    {0,5,3,2,10,11,0},
                    {0,6,4,1,7,8,0},
                    {0,0,0,0,0,9,0}};

char  * moveGrid[6][7]= {{"","ES","SW","ES","SW","S",""},
                        {"","NES","NESW","NESW","NESW","NW",""},
                        {"E","NEW","NSW","","NES","ESW","W"},
                        {"","ES","NWS","NESW","NEW","NSW",""},
                        {"","NW","NW","NE","EW","NSW",""},
                        {"","","","N","","N",""}};
char  const * introMessage = {"Reimdel has fallen. Thieves, convicts and all sorts of low lifes now inhabit that once great village.The king has ordered you and several before you to eradicate those still living there, allowing the village to be re taken, and made once again to the trading centre of Raif al urn."};
char const * familiarMessage = {"You find yourself in a familar place."};
char const * lostMessage = {"You awake to find yourself in a familar place."};
char const * firstDialogue[29] = {"You find yourself standing before an open stone gate. There are no guards in sight. A small blood splatter can be seen along the village wall.\0",
                              "You stand at an open square. Ruined and littered with armour.\0",
                              "A once bustling street stands before you now empty. The smell of blood and smoke fill the air.\0",
                              "Ash and rubble lay before you. Bones of merchants hide beneath the rocks.\0",
                              "You stand in front of a tavern filled with light. Music crossed with destruction can be heard within.\0",
                              "You stand at the back of a tavern filled with light, music crossed with destruction can be heard within.\0",
                              "You stand within a crowded tavern, broken glass and chairs can be seen all across the floor.\0",
                              "You stand before an abandoned guard post, looted and burnt.\0",
                              "You stand before a pile of trash which seems to be covering something.\0",
                              "Behind the trash is a path leading underground, eventually heading to a locked box.\0",
                              "You stand before a burnt stall, an old market place reduced to ashes.\0",
                              "A vile wooden shack is before you, the smell of faeces clogs your lungs.\0",
                              "You stand near the centre of the market. Stalls surprisingly still standing.\0",
                              "You stand before a ladder leading up the castle wall, standing alone and almost untouched\0",
                              "You stand a top the village walls, a peaceful sight of where you journeyed from cut with the sounds of screaming and destruction.\0",
                              "You stand near the centre of the market square, stalls stand to your sides with a faint smell of smoke in the distance.\0",
                              "You stand before a crack in the village wall, all else seems untouched.\0",
                              "It seems as if there is enough room to reach into the wall.\0",
                              "You find yourself before an empty lodging, windows void of light but framed little mould and char.\0",
                              "You stand before a decorative pond, a dead fish can be seen floating near the surface\0",
                              "You stand at a corner of the marketplace, clean stalls with a refreshing air surrounding you.\0",
                              "You stand at the corner of a market place, clean stalls with an odd smell surrounding you\0",
                              "You stand at the top end of the market stalls, untouched yet with an eerie feeling.\0",
                              "You stand at the door of the butcher shop, hanging fresh meat and the smell of rot floods your senses.\0",
                              "You stand before an alchemist, Salves and surgical implements are lined neatly on the desks\0",
                              "A greenhouse is seen in the distance, you stand in rows of strange colourful flowers.\0",
                              "You stand before a water mill, it seems to be functioning yet unmanned.\0",
                              "You stand next to the river entrance, water runs through a gap in the village walls.\0",
                              "You stand before a river passing, a ladder leading into the water can be seen, do you wish to continue?\0"
                              };
char const * secondDialogue[29] ={"\0",
                                   "A common thief reveals himself from hiding and attacks.\0",
                                   "Two knife wielding ruffians leave a stall. They lunge towards you.\0",
                                   "A man sized rat scuttles towards you, hungry and fierce.\0",
                                   "A drunk soldier of a forgotten kingdom sees you. He becomes irate.\0",
                                    "A drunk trio stumbles into you, you get stuck in between them.\0",
                                    "The chatter stops as everyone glares at you, a large group proceed towards you.\0",
                                    "A thief wearing stolen armour exits towards you.\0",
                                    "A horde of bugs fly out of the pile towards you\0",
                                    "The box creaks open and speaks, if you want my treasure then solve my riddle.\0",
                                    "A man on fire bursts out from the ashes, he seems unfazed by his predicament.\0",
                                    "A swarm of flies charge towards you, unable to use their wings.\0",
                                    "Amidst the quiet air, a grotesque beast appears out of nowhere.\0",
                                    "Between the wind, heavy footsteps are heard behind you.\0",
                                    "A chest sits before you.\0", 
                                    "Utter dread creeps in as you stand, you turn around to a robed figure, face hidden.\0",
                                    "Stones begin to fly from the wall, forming a Gollum standing before you.\0",
                                    "\0",
                                    "A robed figure steps out from the building, their face hidden yet emanating with unspeakable horror.\0",
                                    "\0",
                                    "A black mass, devoid of any features emerges from the ground. Its whaling pierces your ears.\0",
                                    "A horse sticks its head out from behind a stall, glaring at you with intent.\0", 
                                    "Thoughts of torment and desire fill your brain as a hooded figure steps out from behind the stalls.\0",
                                    "A decaying mound rises from behind the counter, wielding a cleaver in its hand.\0",
                                    "A large crow like humanoid peers from around the corner. An arm sized scalpel in its beak glints menacingly\0",       
                                    "Rustling can be heard as roots emerge from one of the flower beds.\0",   
                                    "Fear courses through your body as you stand. A robed figure reveals themselves from behind the wheel.\0",   
                                    "Liquid takes form as tendrils rise from the river towards you.\0", 
                                    "Going down, water passes you as if it were air, you go further down onto a dark room, using the river as a roof passing over you.\0" 
                                    };




char const * interactDialogue[28] = {"\0",
                                    "After deep conversation you make him rethink his ways. He lets you pass\0",
                                    "Talking doesn't work, they only seem to be interested in your money.\0",
                                    "In Between squeaks and growls you learn he was once a monk. In the name of God you are let through.\0",
                                    "He ignores your words lunging at you, yet slips and falls to the ground.\0",
                                    "After asking kindly they move away, stumbling into the next person over.\0",
                                    "After ordering a drink everyone calms down again. Only to start tearing the building apart once more.\0",
                                    "After talking you realise he is only borrowing the armour, and proceeds to return it.\0",
                                    "You try to talk, but since they are bugs they cannot understand you\0",      
                                    "What is my name? 1) Reimdel. 2) Fair Lurna. 3) Meirled. 4) Raif al Urn.\0",
                                    "After kindly reminding him that he is aflame, the man thanks you before burning to death.\0",
                                    "You are talking to an army of walking flies... your words pass like music in a storm.\0",
                                    "Your simple kind words are too much for it to understand, it writhes in pain trying to understand your words.\0",
                                    "You turn around, only to see a man with their feet buried in cement. You think best not to get involved.\0",
                                    "Tools to maintain your weapon are found inside, much needed after your journey so far.\0",
                                    "Lacking hostility you speak. The figure gets angry at you, exclaiming how, uniform is important, before walking away.\0",
                                    "After asking the Gollum to go back to the wall as to not let it collapse, it complies.\0",
                                    "Reaching in you feel something bite your hand. You pull it out to see a horrid wound oozing green puss.\0",
                                    "Enquiring about the nightly prices of the establishment, the lone figure helps you before walking away.\0",
                                    "\0",
                                    "It cannot be reasoned with.\0",
                                    "You try to speak, it cuts you off exclaiming the word horse .\0",
                                    "Pretending that you are lost, the hooded figure allows you to try to find your way back.\0",
                                    "Ello, welcome to the lean sausage. U got no money?\0",
                                    "Enquiring about herbal remedies and their available stock, the crow welcomes you to view their shelves.\0",
                                    "Rustling can be heard as roots emerge from one of the flower beds.\0",
                                    "Asking about the use of the wheel, the robed figure enlightens you about the village's famous bread products, often sold in the market.\0",
                                    "Simply standing still, the water seems confused and does nothing, predictable as it has no eyes.\0",
                                    };
char const * secretDialogue[28] = {"\0",
                                    "In his dying breaths he mutters Finally. Oh finally I'm fre-\0",
                                    "Upon closer inspection they're twins. One seems to have been in perfect health, whilst the other is scarred with malformed limbs.\0",
                                    "You have never seen a rat of such size before. Small symbols can be seen carved on its side.\0",
                                    "On the ground he screams of dark cloaked figures and beasts. Rambling of how our kingdom will follow the path his did.\0",
                                    "The wind blows over their lifeless bodies, only to uncover that all three are joined at the waist.\0",
                                    "The smell of blood fills the room, corpses rot in seconds and lifeless eyes glimmer with joy.\0",
                                    "Trying to return the armour, you realise it is fused to their skin.\0",
                                    "Their bodies turn to ash within seconds of falling to the ground.\0",
                                    "The box breaks, destroying the contents within.\0",
                                    "Ancient runes can be seen carved onto his charred chest.\0",
                                    "After inspecting closely, you see that all of their wings have been carefully plucked by hand.\0",
                                    "Black blood spills from its orifices, So dark that all detail loses meaning under its cover.\0",
                                    "Bruised and tortured, many signs of evil play are shown on this person.\0",
                                    "Tools to maintain your weapon are found inside, much needed after your journey so far.\0",
                                    "Guttural screaming projects from its still head, as the rest of the body writhes endlessly. It will not die.\0",
                                    "Inscriptions can be seen on each individual stone, as if drawn by a child.\0",
                                    "There is no gain in attacking a stone wall.\0",
                                    "Guttural screaming projects from its still head, as the rest of the body writhes endlessly. It will not die.\0",
                                    "You raise your sword about to strike. But then realise. The fish is already dead.\0",
                                    "Though its motions have stopped, a thumping beat can be heard within it.\0",
                                    "Human screams are heard as its slowly dies, you have no idea what you just killed\0",
                                    "Guttural screaming projects from its still head, as the rest of the body writhes endlessly. It will not die.\0",
                                    "You can see screaming faces attempting to push through its rotting stomach.\0",
                                    "Black blood leaks from its wounds, whatever was once human is long gone now.\0",
                                    "Black liquid seeps from the cut stems, each plant around you shrivels and dies.\0",                                
                                    "Guttural screaming projects from its still head, as the rest of the body writhes endlessly. It will not die.\0",
                                    "The metal gate, that which only allows the river to run through, seems to have been broken from the inside.\0",
                                 };
                              

char const * comedyDialogue[10] = {"You try talking to the fish. It doesn't respond. As it's already dead.\0",
                                    "You say a joke hoping for some reaction. But the fish is dead.\0",
                                    "It's dead.\0",
                                    "You poke the dead fish to see if it's playing pretend. What's wrong with you?\0",
                                    "It's already dead, why did you try poking the fish?\0",
                                    "It's clearly just a dead fish in the pond.\0",
                                    "You read from the necronomicon in hopes to revive it.\0",
                                    "It's dead.\0",
                                    "Evil forces act upon this village. Battle and death premeditated by strengths unbeknownst to your king. This place has attracted scum of all kinds, acting as the source of power for which this evil requires. Should they be allowed to be let to their own devices, an unstoppable horror will be unleashed upon this world. I have already tried to fight them alongside my companion, yet we failed and were cursed to live as inhuman creatures. Should you fail, run away or at least take your own life, so long as you don't contribute to what these people are trying to summon.\0",
                                    "The fish is dead.\0",};

char const * extraDialogue[30] = {"You find a mysterious vial, a picture of a horse is drawn atop the lid\0",
                                  "Incorrect, goodbye!\0",
                                  "Oh good sir hello, it's been so long since i've been able to talk to anyone. Now I do say you haven't seen a friend of mine have you? Small fellow with a fish like face, and body, and tail for what I remember. Well if you do see him say hi, and most importantly please do stay away from those hooded people, i would hate to see you end up like me.\0",
                                  "10 figures, robed and hidden, can be seen startled, as if you had interrupted something. A malformed black sludge paces towards you, your only option is to fight.\0",
                                  "You defeated the cult's attempts to revive an ancient demon, yet you're ignorant to the evil that remains within the village. You walk back to the king to report your job done.\0",
                                  "10 robed figures can be seen eating lunch, it seems reviving demons is strenuous work. Aware of the situation at hand you take advantage.\0",
                                  "You defeated the cult's attempts to revive an ancient demon, yet you're ignorant to the evil that remains within the village. You decide to stay a bit longer, no serious harm has come your way yet and you almost feel sorry for stopping the cult so easily.\0",
                                  "10 robed figures surround a freshly reborn demon, it stretches its vile black wings, releasing a scream of rage that pierces your ears. It thirsts for blood and heads your way. Your only option is to fight.\0",
                                  "You defeated a new born demon, all signs of the cult seem to be gone or slowly vanishing. Their reach stretches far in this land, but at least this village is safe now. You head back to the king, sword in hand to claim your victory.\0",
                                  "Dismembered bodies can be seen thrown across the room, in the corner is a demon feasting on a robed corpse. Its strength heightens the more it eats. Noticing new life it heads towards you. Your only option is to fight.\0",
                                  "You lay on the ground, bloodied and weakened. All attempts to attack this thing are futile. It grabs you as if its about to feast, giving you a close look at its body. Wiry legs yet with strength to climb mountains, a stomach large enough to contain an army, long wings which could stretch to the heavens, claws that could rip through steel, and a face more disgusting than any abomination imaginable. You pray for a merciless end to any that may face it.\0",
                                  "Reimdel has fallen. Thieves, convicts and all sorts of low lifes now inhabit that once great village. The king has ordered you and several before you to eradicate those still living there, allowing the village to be re-taken, and made once again to the trading centre of Raif al urn.\0"};
 
int playerX = 3;
int playerY = 5;
int oldPlayerX = 3;
int oldPlayerY = 5;

int len(char * array){
    int counter = 0;
    for(int i = 0; i <100; i++){
        if (array[i] == '\0'){
            
            break;}
        else {counter +=1;}
        }
        return counter;
    }

void wait(int time){  // shorter version of thread_sleep
    thread_sleep_for(time);
} 
float mod(float num, float den){
    float div = num/ den;
    float decRem = div-floor(div);
    return decRem*den;
}
void getDifficulty(){
   if (playerY ==0 | playerY ==4 |playerX ==1 |playerX== 5){
       difficulty = 1;
   }else{
       difficulty = 2;
   }
   if(playerX == 0 & playerY == 2){
       difficulty = 3;
   }
   if (playerX == 3 & playerY == 5){
       difficulty = 9;
   }
}
void playSFX(int SFXno1, int SFXno2, int SFXno3){
    SFXBang = 0;
    thread_sleep_for(500);   
    SFXnumber1 = SFXno1;
    SFXnumber2 = SFXno2;
    SFXnumber3 = SFXno3;
    thread_sleep_for(200);
    SFXBang = 1;
}
void playSong(int songNo){
    if (songNo != musicNo){
    musicBang = 0;
    thread_sleep_for(1000);
    musicNo = songNo;
    thread_sleep_for(200);
    musicBang = 1;}
}

void waitForButtonPress(){
    continueSemaphore.try_acquire();
    continueSemaphore.acquire();
    button1Down = false;
}
void sansWrite(char * stringOut){  //writes out the input letter by letter
    lcd.cls();
    for(int i = 0; i < 25  ; i++ ){
        if (stringOut[i]!= '\0'){
            if (i == 16){
                lcd.locate(0,1);
            }
           // char letter[1] = {stringOut[i]};
            wait(200);
        }else{
            break;
        }
    }
    
};
void write(char const *stringOut){  //writes out the input letter by letter
    lcd.cls();
    lcd.printf("%s",stringOut);
    thread_sleep_for(700);  
};
void writeSquare(){
    lcd.cls();
    lcd.printf("Square: %c %d", (playerX+65), playerY + 1);
    thread_sleep_for(1000);
}
void writeDialogue(char const * info){
    bool end = false;
    int i = 0;
    int endIndex = 0;
    while (end == false){
        if (info[i] == '\0'){
            endIndex = i;
            end = true;
        }else{i++;
        };
    }
    end = false;
    int latSpace = 0;
    int oldSpace = 0;
    char  textOut[1000];
    textOut[17] = '-';
    textOut[18] = '-';
    textOut[19] = '-';
    while (end == false){
        oldSpace = latSpace;
        for (int j = 0; j <16; j++){
            if (info[oldSpace +j] == ' '){
                latSpace = oldSpace +j;
            }
            if (info[oldSpace +j] == '\0'){
                latSpace = oldSpace +j;
                end = true;
                break;
            }
        }
        for(int l = 0; l < 32; l++){
            textOut[l]= ' ';
        }
        if (oldSpace == 0){
        for (int l = oldSpace ; l < latSpace; l++){
            textOut[l - oldSpace] = info[l];
        }}else{for (int l = oldSpace + 1; l < latSpace; l++){
            textOut[l - oldSpace -1] = info[l];
        }
        }
        oldSpace = latSpace;
        for (int j = 0; j <16; j++){
            if (info[oldSpace +j] == ' '){
                latSpace = oldSpace +j;
            }
            if (info[oldSpace +j] == '\0'){
                latSpace = oldSpace +j;
                end = true;
                break;
            }
        }
        for (int l = oldSpace + 1; l < latSpace; l++){
     
            textOut[l - oldSpace +19] = info[l];
        }    
        if (end == false){
            textOut[19+latSpace-oldSpace] = '>';
            textOut[19+latSpace-oldSpace+1] = '\0';
        }else{
            textOut[19+latSpace-oldSpace] = '\0';
        }
        write(textOut);
    }
}
void flashWrite(char stringOut[32]){ //writes out the input letter by letter then flashes it 
    lcd.cls();
    write(stringOut);
    wait(500);
    lcd.cls(); 
    wait(250);
    lcd.printf("%s", stringOut);
    wait(250);
    lcd.cls(); 
    wait(250);
    lcd.printf("%s", stringOut);
    wait(250);
    lcd.cls(); 
    wait(250);
    lcd.printf("%s", stringOut);
    wait(250);
    lcd.cls(); 
} 
void createMoveOption(int currentOpt, int counter, char * outputstring){
    outputstring[16] = '|';
    outputstring[17] = '|';
    outputstring[18] = '|';
    outputstring[19] = '|';
    outputstring[20] = '|';
    outputstring[35] = '|';
    outputstring[37] = '\0';
    for (int i = 21; i < 35; i++){
        outputstring[i] = ' ';
    }
    if(counter ==4){
        outputstring[23+(currentOpt*3)]= '-';       
    }
    if(counter ==3){
        outputstring[23+(currentOpt*4)]= '-'; 
    }
    if(counter == 2){
        if (currentOpt == 0){
            outputstring[25] = '-';
        }else{
            outputstring[30] = '-';   
    }
    }
    if (counter == 1){
        for (int i = 24; i < 32;i++){
            outputstring[i] = '-';
        }
    }    
    }

void getMove(char moves[5]){
    int counter = 0;

    for (int i = 0; i < 5; i++){
        if (moves[i]== '\0'){
            break;
        }else{ 
            counter+=1;
        }
    }
    char outputstring[37];
    if (counter == 4){
        outputstring[0] = '|';
        outputstring[1] = ' ';
        for (int i = 3; i< 14;i+=3){
            outputstring[i-1]=' ';
            outputstring[i]=moves[((i-2)/3)];
            outputstring[i+1]=' ';
        }
        outputstring[14] = ' ';
        outputstring[15] = '|';
        outputstring[16] = '\0';
    }
    if (counter == 3){
        outputstring[0] = '|';
        for (int i = 3; i< 15;i+=4){
            outputstring[i-2]=' ';
            outputstring[i-1]=' ';
            outputstring[i]=moves[((i-3)/4)];
            outputstring[i+1]=' ';
        }
        outputstring[13] = ' ';
        outputstring[14] = ' ';
        outputstring[15] = '|';
        outputstring[16] = '\0';
    }
    if (counter == 2){
        outputstring[0] = '|';
        outputstring[1] = ' ';
        outputstring[2] = ' ';
        outputstring[3] = ' ';
        outputstring[4]= ' ';
        outputstring[5] = moves[0];
        outputstring[6] = ' ';
        outputstring[7] = ' ';
        outputstring[8] = ' ';  
        outputstring[9]= ' '; 
        outputstring[10] = moves[1];
        outputstring[11] = ' '; 
        outputstring[12] = ' ';
        outputstring[13] = ' ';
        outputstring[14] = ' ';
        outputstring[15] = '|';
        outputstring[16] = '\0';
    }
    if (counter ==1){
        outputstring[0] = '|';
        outputstring[1] = ' ';
        outputstring[2] = ' ';
        outputstring[3] = ' ';
        outputstring[4]= 'c';
        outputstring[5] = 'o';
        outputstring[6] = 'n';
        outputstring[7] = 't';
        outputstring[8] = 'i';  
        outputstring[9]= 'n'; 
        outputstring[10] = 'u';
        outputstring[11] = 'e'; 
        outputstring[12] = ' ';
        outputstring[13] = ' ';
        outputstring[14] = ' ';
        outputstring[15] = '|';
    }
    int currentOpt = 0;
    lcd.cls();
    createMoveOption(currentOpt, counter, outputstring);
    lcd.printf("%s", outputstring);
    while (button1Down == false){
       if((ainX > 0.8) && (currentOpt < counter -1)){
           currentOpt +=1;
            lcd.cls();
            createMoveOption(currentOpt, counter, outputstring);
            lcd.printf("%s", outputstring);
            stickBackSemaphore.acquire();
        }
        if((ainX < 0.3) && (currentOpt > 0)){   
            currentOpt -=1;
            lcd.cls();
            createMoveOption(currentOpt, counter, outputstring);
            lcd.printf("%s", outputstring);
            stickBackSemaphore.acquire();
        }
    }
    oldPlayerX = playerX;
    oldPlayerY = playerY;
    if(moves[currentOpt] == 'S'){
        playerY +=1;
        }else if (moves[currentOpt] == 'N'){
            playerY -=1;
        }else if (moves[currentOpt] == 'W'){
            playerX -=1;
        }else if (moves[currentOpt] == 'E'){
            playerX +=1;
        }  
}
bool getYesOrNo(){ //will need an implementation to allow the user to view the question again
  
    bool yOrN = true;
    lcd.cls();          
    lcd.printf("|   yes   no   |abc |   ---        |");
      button1Down = false;
    while (button1Down == false){
       if((ainX > 0.8) && (yOrN == true)){
            lcd.cls();
           yOrN = false;
            lcd.printf("|   yes   no   |abc |         --   |");
        }
        if((ainX < 0.3) && (yOrN == false)){   
            lcd.cls();
            yOrN = true;
            lcd.printf("|   yes   no   |abc |   ---        |");
        }
    }
    button1Down = false;


    if(yOrN == true){
        lcd.cls();
        lcd.printf("you picked yes");
        wait(500);
        return true;
    }else{
        lcd.cls();
        lcd.printf("you picked no");
          wait(500);
        return false;}

}
void button1CallBack(){
     button1Semaphore.release();  
     waveCount = 0;
     
}
void button1ThreadFn(){
    while(true){
    button1Semaphore.acquire();
    continueSemaphore.release();
    int X = floor(ainX*1000);
    button1Down = true;
    thread_sleep_for(10);
    button1Down = false;
    }
}
void stickBackThreadFn(){
    while(true){
    if (ainX > 0.45 && ainX < 0.55 && ainY > 0.45 && ainY < 0.55){
        stickBackSemaphore.release();
    }
};
}
void waveCountFn(int pastFreq){
    if (freq > pastFreq){
        if (goingUp == false){
            waveCount+=1;
            if (waveCount >10){
                waveCount = 5;
                lostHealth = true;
                }
            }
        goingUp = true;
        }
    else{goingUp = false;}
    }

void purrThreadFn(){
    while(true){
        getDifficulty();
        int pastFreq = freq;
        freq = 550 + round(450*cos((mod(timerVal,(1/0.7))/ (1/0.7)) * 2* 3.142));
        if (freq > 999){
            freq =999;
        }else if(freq <100){
            freq = 100;
        }
        if (start == 0){
            freq = 500;
        }
        waveCountFn(pastFreq);
        if (score == 10  ){
            score = 9;
            fighting = false;
        }
        if (score <1){
            score = 0;
            fighting = false;
        }

         printf("1234 %d %d %d %d %d %d %d %d %d ", freq, score, audioOn , SFXnumber1, SFXnumber2, SFXnumber3, SFXBang, musicNo, musicBang);
     
        thread_sleep_for(20);
    
    }
}
bool goBackOption(){
    int currentOpt = 0;
    lcd.cls();
    lcd.printf("|continue  back|----|--------      |");


    while (button1Down == false){
       if((ainX > 0.8) & (currentOpt == 0)){
       
           currentOpt =1;
            lcd.cls();
            lcd.printf("|continue  back|----|          ----|");
          
            stickBackSemaphore.acquire();
        }

        if((ainX < 0.3) & (currentOpt == 1)){   
            currentOpt =0;
            lcd.cls();
            lcd.printf("|continue  back|----|--------      |");
           
            stickBackSemaphore.acquire();
        }
    }
    if (currentOpt == 0){
        return false;
    }else{
        return true;
    }
}
bool fightOption(){
    int currentOpt = 0;

    lcd.cls();
    lcd.printf("|interact fight|----|--------      |");
    

    while (button1Down == false){
       if((ainX > 0.8) && (currentOpt == 0)){
           currentOpt =1;
            lcd.cls();
            lcd.printf("|interact fight|----|         -----|");
            stickBackSemaphore.acquire();
        }
        if((ainX < 0.3) && (currentOpt == 1)){   
            currentOpt =0;
            lcd.cls();
            lcd.printf("|interact fight|----|--------      |");
            stickBackSemaphore.acquire();
        }
    }
    if (currentOpt == 0){
        return false;
    }else{
        return true;
    }
}
void soundStutter(int time){
    timerPause = true;
    timerStop = true;
    for(int i = 0; i <3;i++){
        audioOn = 0;
        thread_sleep_for(time/6);
        audioOn = 1;
        thread_sleep_for(time/6);
    }
    timerPause = false;
}
bool fight(){ 
    fighting = true;
    start = 1;
    score = 6;
    thread_sleep_for(100);
    audioOn = 1;
    resetOscBang = 1;
    thread_sleep_for(10);
    resetOscBang = 0;
    while(fighting ==  true){       
        lcd.cls();
        lcd.printf("S| ");
        for(int i = 0; i < score; i++){
                lcd.printf("-");
            }
        for (int i = score; i < 10; i++){
                lcd.printf(" ");
        }
        lcd.printf("|");
        bangWin = 0;
        bangLose = 0;
        waveCount = 0;
        timerStartSemaphore.release();
        while(button1Down == false & lostHealth == false){
            thread_sleep_for(3);
            if (fighting == false){
                break;
            }
        }
        if (fighting == false){
                break;
            }
        if (lostHealth == false){
            if (freq > 500){ // good hit   
                if (buff == 1){
                    green = true;
                    soundStutter(500);
                    green = false;
                    score +=1;
                    if(randomNo > 80){
                        score+=1; //double hit from buff
                    }
                    bangWin = true;
                    
                }else if (buff == -1){                                  
                    if(randomNo > 80){
                         yellow = true;
                        soundStutter(500);//when player gets no hit from debuff
                        yellow = false;
                        bangWin = true;
                        //playSFX(1);
                    }else{
                        green = true;
                        soundStutter(500);
                        score+=1;
                        green = false;
                        bangWin = true;
                        
                    }
                    
                }else{// good hit
                    green = true;
                    soundStutter(500);
                    green = false;
                    score +=1;
                    bangWin = true;
                    
                }              
            }else{// bad hit
                red = true;
                soundStutter(500);
                red = false;
                score -=2;
                bangLose = true;
                
            }
            
        }else{//player waited too long
            red = true;
            thread_sleep_for(200);
            red = false;
            score-=1;
            lostHealth = false;
            bangLose = true;
        }
    }
    waveCount = 0;
    audioOn = 0;
    if (score > 1){
        
        flashWrite("S |----------|");
        return true;
    }else{
       
        write("     X   X      ----       O");
        return false;
    }
}

void timerThreadFn(){
    while(true){
    timerStartSemaphore.acquire();
    timerStop = false;
    timerVal = 0;
    float mult = 0;
    if(difficulty == 1){
        mult = 1;
    }else if(difficulty == 2){
        mult = 1.5;
    }else{mult = 2;}
    while(timerStop == false){
        thread_sleep_for(10);
        timerVal += 0.03* mult;
        while(timerPause == true){
            thread_sleep_for(3);
        }
    } }
}
void randomNoThreadFn(){
    while(true){
    randomNo +=1;
    if (randomNo == 100){
        randomNo = 0;
    }
    thread_sleep_for(10);
   }
}

bool runFight(){

    if (fight() == true){//if wins
        if (randomNo > 60){
            writeDialogue(secretDialogue[diGrid[playerY][playerX]]);
            choiceData[1] +=1;
        }
        visited[playerY][playerX] = 1;
        getMove(moveGrid[playerY][playerX]);
        return true;
    }else{
        writeDialogue(lostMessage);
        return false;}// if loses
        
}
int get1234Opt(){
        int currentOpt = 1;

    lcd.cls();
    lcd.printf("|  1  2  3  4  |----|  -           |");
    while (button1Down == false){
       if((ainX > 0.8) && (currentOpt < 4)){
           currentOpt +=1;
            lcd.cls();
                        if (currentOpt == 1){       
                lcd.printf("|  1  2  3  4  |----|  -           |");    
            }else if(currentOpt == 2){
                lcd.printf("|  1  2  3  4  |----|     -        |");    
            }else if(currentOpt == 3){
                lcd.printf("|  1  2  3  4  |----|        -     |");    
            }else{
                lcd.printf("|  1  2  3  4  |----|           -  |");    
            }
            stickBackSemaphore.acquire();
        }
        if((ainX < 0.3) && (currentOpt > 1)){   
            currentOpt -=1;
            lcd.cls();
            if (currentOpt == 1){       
                lcd.printf("|  1  2  3  4  |----|  -           |");    
            }else if(currentOpt == 2){
                lcd.printf("|  1  2  3  4  |----|     -        |");    
            }else if(currentOpt == 3){
                lcd.printf("|  1  2  3  4  |----|        -     |");    
            }else{
                lcd.printf("|  1  2  3  4  |----|           -  |");    
            }
            stickBackSemaphore.acquire();
        }
    }
    return currentOpt;
}
void playCredits(){
    while (true){

    }
}
void runBossRoom(){
    int topNum = 0;
    for (int i = 1; i<4; i++){
        if (choiceData[1] > choiceData[topNum]){
            topNum = i;
        }
    }
    if (choiceData[1] == 15){
        topNum = 1;
    }
    if(topNum == 0){
        //player went back most
        writeDialogue(extraDialogue[3]);
        if (runFight() == true){
            playSong(5);
            writeDialogue(extraDialogue[4]);
            playCredits();
        };
    }else if(topNum == 1){
        //secretDialogue
        writeDialogue(extraDialogue[9]);
        if (runFight() == true){
            playSong(5);
            writeDialogue(extraDialogue[10]);
            playCredits();
        };
        
    }else if(topNum == 2){
        //interact
        writeDialogue(extraDialogue[5]);
        if (runFight() == true){
            playSong(5);
            writeDialogue(extraDialogue[6]);
            playCredits();
        };
    }else if(topNum == 3){
        //fight
        writeDialogue(extraDialogue[7]);
        if (runFight() == true){
            playSong(5);
            writeDialogue(extraDialogue[8]);
            playCredits();
        };
    }
}
void writeSecretDialogue(int dialogueNo){
    if (randomNo > 60){
        writeDialogue(secretDialogue[dialogueNo]);
    }
}
int main(){
    
    button1.rise(button1CallBack);
    stickBackThread.start(stickBackThreadFn);
    button1Thread.start(button1ThreadFn);
    purrThread.start(purrThreadFn);
    timerThread.start(timerThreadFn);
    randomNoThread.start(randomNoThreadFn);
    lcd.cls();          //clears whole screen
    lcd.locate(0,0);    //moves cursor, rows and columns are indexed 0-1 and 0-1
    bool end = false;    
    playerX = 3;
    playerY = 4;
    playSong(1);
    lcd.printf("   LEGEND  OF   ----   COUNT  DAN   ");
    waitForButtonPress();
    lcd.cls();
    playSFX(0,0,0);
    writeDialogue(extraDialogue[11]);
  
    
        while(end == false){
            playSong(2);
            writeSquare();
            if( visited[playerY][playerX] == 1){
                writeDialogue(familiarMessage);
            }

            writeDialogue(firstDialogue[diGrid[playerY][playerX]]);
                
            if(goBackOption() == true){
                choiceData[0]+=1;
                playerX = oldPlayerX;
                playerY = oldPlayerY;  //returns to previous square
            }else if (visited[playerY][playerX] == 0){ //if haven't visited
                if (difficulty != 9){
                    writeDialogue(secondDialogue[diGrid[playerY][playerX]]);
                if (playerX == 5 & playerY == 0){  // comedy square F-1
                        bool exitloop = false;
                        int comedyNo = 0;
                        while (exitloop == false){
                            if (comedyNo == 11){
                                comedyNo = 0;
                            }
                            if (fightOption() == true){
                                playSong(difficulty +2);
                                choiceData[3] +=1;
                                writeSecretDialogue(19); 
                                visited[playerY][playerX] = 1;
                                getMove(moveGrid[playerY][playerX]);  
                                exitloop = true;
                            }else{
                                choiceData[2] +=1;
                                writeDialogue(comedyDialogue[comedyNo]);  
                                comedyNo +=1;
                            }
                        }
                    }else if(playerX == 5 & playerY == 5){//potion F-6
                        if (fightOption() == true){
                                playSong(difficulty +2);
                                choiceData[3] +=1;
                                writeSecretDialogue(9); 
                                visited[playerY][playerX] = 1;
                                getMove(moveGrid[playerY][playerX]);                              
                            }else{
                                choiceData[2] +=1;
                                writeDialogue(interactDialogue[9]);
                                if (get1234Opt()==3){
                                    potion = true;                                    
                                    writeDialogue(extraDialogue[0]);
                                    visited[playerY][playerX] = 1;  
                                    getMove(moveGrid[playerY][playerX]);       
                                }else{                                                  
                                    writeDialogue(extraDialogue[1]);
                                    getMove(moveGrid[playerY][playerX]);  
                                };
                            }
                    }else if (playerX == 0 & playerY == 2){//A-3
                        if (fightOption() == true){
                            playSong(difficulty +2);
                            choiceData[3] +=1; 
                            writeSecretDialogue(diGrid[playerY][playerX]);
                            visited[playerY][playerX] = 1;  
                            getMove(moveGrid[playerY][playerX]);
                        }else{
                            choiceData[2] +=1;
                            writeDialogue(interactDialogue[diGrid[playerY][playerX]]);
                            visited[playerY][playerX] = 1;  
                            getMove(moveGrid[playerY][playerX]);
                            buff-=1;
                        }    

                    }else if (playerX == 2 & playerY == 1){//C-2
                        if (fightOption() == true){
                            playSong(difficulty +2);
                            choiceData[3] +=1; 
                            runFight();
                        }else{
                            choiceData[2] +=1;
                            if (potion == true){
                                writeDialogue(extraDialogue[2]);
                                visited[playerY][playerX] = 1;  
                                getMove(moveGrid[playerY][playerX]);
                            }else{
                                 writeDialogue(interactDialogue[diGrid[playerY][playerX]]);
                                 visited[playerY][playerX] = 1;  
                                getMove(moveGrid[playerY][playerX]);
                            }
                        }    

                    }else if (playerX == 6 & playerY == 3){//G-3
                        if (fightOption() == true){
                            playSong(difficulty +2);
                            choiceData[3] +=1; 
                            visited[playerY][playerX] = 1;
                            writeDialogue(interactDialogue[14]);
                            buff+=1;
                        }else{                
                            choiceData[2] +=1;   
                            visited[playerY][playerX] = 1;
                            writeDialogue(interactDialogue[14]);
                            buff+=1;
                        }    

                    }else if (playerX ==3 & playerY == 2){//BOSS ROOM
                            runBossRoom();
                    }else{//regular square
                        if (fightOption() == true ){//if decides to fight
                            playSong(difficulty +2);
                            choiceData[3] +=1; 
                            runFight();
                        }else{// if decides to interact    
                            choiceData[2] +=1;                      
                            writeDialogue(interactDialogue[diGrid[playerY][playerX]]);
                            if (forceFight[playerY][playerX] == 1){
                                playSong(difficulty +2);
                                runFight();   
                            }else{
                                visited[playerY][playerX] = 1;
                                getMove(moveGrid[playerY][playerX]);                  
                            }
                }  }
                      
                }else{playerY -=1;}
            }else{
                getMove(moveGrid[playerY][playerX]);    
                }
                }
                }
         
