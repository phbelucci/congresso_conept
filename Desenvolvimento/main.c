#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdbool.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <locale.h>

#define     FPS                     60
#define     TELA_LARGURA            1920
#define     TELA_ALTURA             1080
#define     TOLERANCIA              5
#define     TEMPO_JOGO_SEGUNDOS     60
#define     TEMPO_EXPLOSAO          15  //tempo em segundos = TEMPO_EXPLOSAO/60
#define     TELA_MENU               0
#define     TELA_ESCOLHA            1
#define     TELA_JOGO               2
#define     TELA_PAUSE              3
#define     TELA_RANKING            4
#define     TELA_FIM_TEMPO          5
#define     NUM_JOGADORES           10
#define     TAMANHO_NOME_PLAYER     10
#define     NUM_TIROS               15

//----------------------------------------------------DECLARAÇÃO DE VARIÁVEIS GLOBAIS
// Ponteiro representando a janela principal
ALLEGRO_DISPLAY *janela = NULL;
//Poteiro que recebe  o TIMER do programa
ALLEGRO_TIMER *timer = NULL;
//Matriz para receber as naves e seus respectivos estados de destruição
ALLEGRO_BITMAP *naveImagem[3][3];
//Estrutura que representa os objetos do jogo(Nave, obstáculos, botões e etc.)
typedef struct{
    int posX;
    int posY;
    int xColisao;
    int yColisao;
    int x1Colisao;
    int y1Colisao;
    int largura;
    int altura;
    int temp;
    float vel;
    float ang;
    bool ativo;
    ALLEGRO_BITMAP *objetoBitmap;
    ALLEGRO_FONT *objetoFont;
}Objeto;
// Declaração dos objetos do jogo
Objeto backGround;
Objeto backGroundExplo;
Objeto backGroundCidade;
Objeto cenario1;
Objeto cenario2;
Objeto cenario3;
Objeto backGroundMenu;
Objeto backGroundEscolha;
Objeto backGroundRanking;
Objeto backGroundPause;
Objeto backGroundFimTempo;
Objeto nave;
Objeto tiro[NUM_TIROS];
Objeto obst;
Objeto obst2;
Objeto obst3;
Objeto obst4;
Objeto explo;
Objeto exploObj;
Objeto exploObj2;
Objeto exploObj3;
Objeto exploObj4;
Objeto vilao;
Objeto btPlay;
Objeto btExit;
Objeto btPlayAgain;
Objeto btQuit;
Objeto btNaveZero;
Objeto btNaveUm;
Objeto btNaveDois;
Objeto btCenarioEspaco;
Objeto btCenarioCidade;
Objeto btPlayJogo;
Objeto btPlayPause;
Objeto btExitPause;
Objeto vida1;
Objeto vida2;
Objeto vida3;
Objeto plusTime;
Objeto plusScore;
Objeto textTime;
Objeto textRank;
Objeto textScoreJogo;
Objeto textPlayer;
Objeto ultimaChance;


//Declara e inicializa Variaveis de controle do jogo
char ultChTexto[10];
bool sair = 0;
bool pause = 0;
bool jogoSalvo = 0;
int score = 0;
int somaScore = 0;
int tempJogo=0;
int telaAtual=TELA_MENU;
int vidas = 3;
char nomePlayer[TAMANHO_NOME_PLAYER]={"SEU NICK"};
int indexNome = -1;
int caracterPendente = 0;  // 0 - Não há letra a ser lida   e 1 - Letra  aguardando ser lida
char caracter='\0';    // Armazena o caractere digitado
bool teclaEnter = 0;
bool teclaApagar = 0;
int rTime = 0;
int gTime = 255;
int bTime = 255;
int progressao = 10;
int tipoNave = 0;
int estadoNave = 0;
int naveClick = 0;
int cenarioClick = 0;
int rankingScore[NUM_JOGADORES],i;
char rankingPlayers[NUM_JOGADORES][50];
int limitTiro = 0;
//Declara e inicializa Variaveis com a posição X dos objetos
int posXMouse = 4000;
int posXMouseClick = 4000;
//Declara e inicializa Variaveis com a posição Y dos objetos
int posYMouse = 4000;
int posYMouseClick = 4000;
//Declara e inicializa Variavel de incremento/decremento para deslocamento vertical dos objetos
float velDesloc = 8;
//Declara e inicializa Variavel para contagem de tempo
int tempExploNave = 4000;
int posColisao = 4000;
int piscaNave=0;
int tempTiro = 0;
int tempTelaFimTempo = 0;
//Declara e inicializa Variaveis de controle do teclado
bool cima = 0;
bool baixo = 0;
bool freia = 0;
bool acelera = 0;
int disparo =0;

/*----------------------------------------------------
ATRIBUI OS VALORES INICIAIS AS VARIÁVEIS RELACIONADAS
A TELA JOGO ANTES DE INICIAR UMA NOVA PARTIDA
----------------------------------------------------*/
void resetJogo();
/*----------------------------------------------------
VERIFICA SE HOUVE COLISAO ENTRE OS OBJETOS PASSADOS
POR PARÂMETRO E RETORNA VERDADEIRO CASO HOUVE, E FALSO
CASO NÃO HOUVE.
----------------------------------------------------*/
bool colisao(Objeto obj1, Objeto obj2);
/*----------------------------------------------------
VERIFICA SE O CURSOR DO MOUSE ESTÁ POSICIONADO SOBRE
O BOTÃO PASSADO POR PARÂMETRO E RETORNA 1 CASO ESTEJA
E 0 CASO NÃO ESTEJA
----------------------------------------------------*/
int mousePosicao(ALLEGRO_BITMAP *botao, int posXBotao, int posYBotao);
/*----------------------------------------------------
VERIFICA SE OCORREU UM CLICK NO BOTÃO PASSADO POR
PARÂMETRO
----------------------------------------------------*/
int mouseClick(ALLEGRO_BITMAP *botao, int posXBotao, int posYBotao);
/*----------------------------------------------------
CARREGA AS IMAGENS(BITMAPS) DO JOGO EM SEUS RESPECTIVOS
PONTEIROS
----------------------------------------------------*/
void loadBitmap();
/*----------------------------------------------------
EXIBE A TELA INICIAL COM AS OPÇÕES DE JOGAR E SAIR
----------------------------------------------------*/
void telaMenu();
/*----------------------------------------------------
EXIBE A TELA PARA ESCOLHER 1 ENTRE 3 OPÇÕES DE NAVE,
1 ENTRE DUAS OPÇÕES DE CENÁRIO E INFORMAR O NOME(NICK)
DO PLAYER
----------------------------------------------------*/
void telaEscolha();
/*----------------------------------------------------
EXIBE UMA TELA DE PAUSE COM AS OPÇÕES DE CONTINUAR
JOGANDO OU SAIR DO JOGO
----------------------------------------------------*/
void telaPause();
/*----------------------------------------------------
ABRE ARQUIVO E CARREGA OS DADOS DO MESMO NOS VETORES
PASSADOS POR PARÂMETRO.
CASO NÃO HAJA ARQUIVO, CRIA UM NOVO.
EM SEGUIDA FECHA O ARQUIVO.
----------------------------------------------------*/
int abreArquivoDisco(char rankingPlayersArq[][50], int numJogadores, int scoreArq[]);
/*----------------------------------------------------
ABRE O ARQUIVO EXISTENTE E SALVA OS DADOS DO JOGO
PASSADOS POR PARÂMETRO NO MESMO. EM SEGUIDA FECHA O
ARQUIVO.
----------------------------------------------------*/
int SalvaArquivoDisco(char rankingPlayersArq[][50], int numJogadores, int scoreArq[]);
/*----------------------------------------------------
RECEBE OS VETORES DE PONTUAÇÃO E NOMES DOS JOGADORES E
ORDENA AMBOS DA MAIOR PONTUAÇÃO PARA A MENOR UTILIZANDO
O ALGORITMO DE ORDENAÇÃO INSERTION SORT.
----------------------------------------------------*/
void ordenaRanking(int *ranking[], char nomeJogadores[][50]);
/*----------------------------------------------------
CARREGA O RANKING A PARTIR DO ARQUIVO, EXIBE O RANKING
ORDENADO E AS OPÇÕES DE JOGAR NOVAMENTE OU SAIR
----------------------------------------------------*/
void telaRanking();
/*----------------------------------------------------
DESENHA AS IMAGENS CORRESPONDENTES A TELA JOGO
----------------------------------------------------*/
void drawTelaJogo ();
/*----------------------------------------------------
EXECUTA A MECÂNICA E A LÓGICA DO JOGO, EXIBINDO OS OBJETOS
EM MOVIMENTO NA TELA.
----------------------------------------------------*/
void telaJogo();
/*----------------------------------------------------
EXIBE A MENSAGEM DE QUE ACABOU O TEMPO E CHAMA A TELA
RANKING.
----------------------------------------------------*/
void telaFimTempo();
/*----------------------------------------------------
ATRIBUI OS VALORES INICIAIS AS VARIÁVEIS GLOBAIS
RELACIONADAS A TODAS AS TELAS.
----------------------------------------------------*/
void inicializaObjetos();
/*----------------------------------------------------
FUNÇÃO PRINCIPAL: INICIALIZA OS PARÂMETRO DA ALLEGRO
NECESSÁRIOS PARA O FUNCIONAMENTO DO JOGO.
CALCULA RESOLUÇÃO DO JOGO EM FUNÇÃO DA RESOLUÇÃO DA TELA
DO COMPUTADOR UTILIZADO.
----------------------------------------------------*/
int main(void){
    //------------------------------------- inicializacao Bibl. ALLEGRO -------------------------------------
    al_init();
    //------------------------------------- Variaveis para ENVENTOS -------------------------------------
    //Variavel ponteiro para fila de enventos
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    //------------------------------------- inicializacao JANELAS e IMAGENS ---------------------------------
    // Inicializa o add-on para utilização de imagens
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();


    // Configura a janela

    ALLEGRO_MONITOR_INFO info;

	int res_x_comp, res_y_comp;

	al_get_monitor_info(0, &info);

	res_x_comp = info.x2 - info.x1;
	res_y_comp = info.y2 - info.y1;

	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	janela = al_create_display(res_x_comp, res_y_comp);

	float red_x = res_x_comp / (float) TELA_LARGURA;
	float red_y = res_y_comp / (float) TELA_ALTURA;

	ALLEGRO_TRANSFORM transformar;

	al_identity_transform(&transformar);
	al_scale_transform(&transformar, red_x, red_y);
	al_use_transform(&transformar);

    //------------------------------------- inicializacao EVENTOS---------------------------------
    //Inicializa variável timer com FPS definido
    timer = al_create_timer(1.0 / FPS);

    //inicializa addon do teclado
    al_install_keyboard();

    //inicializa addon do mouse
    al_install_mouse();

    // Atribui o cursor padrão do sistema para ser usado
    al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);

    //Inicializa variável fila de eventos
    fila_eventos = al_create_event_queue();

    //registra  timer, teclado e mouse na fila de eventos
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));

    //inicia a contagem do timer
    al_start_timer(timer);

    //Cria variavel para receber o tipo de evento detectado
    ALLEGRO_EVENT evento;

    inicializaObjetos();
    loadBitmap(); //carrega imagens para o jogo

    while(!sair){
        //Aguarda pelo proximo evento e, quando ocorrer, pega o ultimo evento da fila e atribui a variavel evento
        al_wait_for_event(fila_eventos, &evento);

        //****************************************************************************
        //Se o evento for o timer...
        //****************************************************************************
        if(evento.type == ALLEGRO_EVENT_TIMER){

            if(telaAtual==TELA_MENU){
                telaMenu();
            }else if (telaAtual==TELA_ESCOLHA){
                telaEscolha();
            }else if(telaAtual==TELA_JOGO){
                telaJogo();
            }else if(telaAtual == TELA_PAUSE){
                telaPause();
            }else if(telaAtual == TELA_FIM_TEMPO){
                telaFimTempo();
            }else if(telaAtual == TELA_RANKING){
                telaRanking();
            }

            //Atualiza a tela
            al_flip_display();
        }

        //****************************************************************************
        //se o evento for PRESSIONAR uma tecla...
        //****************************************************************************
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
            //verifica qual tecla foi pressionada
            switch(evento.keyboard.keycode){
                //seta para cima
                case ALLEGRO_KEY_UP:
                cima = 1;
                break;
                //seta para baixo
                case ALLEGRO_KEY_DOWN:
                baixo = 1;
                break;
                //seta para esquerda
                case ALLEGRO_KEY_LEFT:
                freia = 1;
                break;
                //seta para direita.
                case ALLEGRO_KEY_RIGHT:
                acelera = 1;
                break;
                //esc. sair=1 faz com que o programa saia do loop principal
                case ALLEGRO_KEY_ESCAPE:
                switch(telaAtual){
                    case TELA_MENU:
                    sair=1;
                    break;
                    case TELA_ESCOLHA:
                    sair=1;
                    break;
                    case TELA_JOGO:
                    telaAtual=TELA_PAUSE;
                    break;
                    case TELA_PAUSE:
                    telaAtual=TELA_JOGO;
                    break;
                    case TELA_RANKING:
                    sair = 1;
                    break;
                }
            case ALLEGRO_KEY_BACKSPACE:
            teclaApagar = 1;
            break;
            case ALLEGRO_KEY_ENTER:
            teclaEnter = 1;
            break;
            }
        }

        //****************************************************************************
        //se o evento for SOLTAR uma tecla...
        //****************************************************************************
        if (evento.type == ALLEGRO_EVENT_KEY_UP){
            //verifica qual tecla foi pressionada
            switch(evento.keyboard.keycode){
                //seta para cima
                case ALLEGRO_KEY_UP:
                cima = 0;
                break;
                //seta para baixo
                case ALLEGRO_KEY_DOWN:
                baixo = 0;
                break;
                //seta para esquerda
                case ALLEGRO_KEY_LEFT:
                freia = 0;
                break;

                //seta para direita.
                case ALLEGRO_KEY_RIGHT:
                acelera = 0;
                break;
                case ALLEGRO_KEY_BACKSPACE:
                teclaApagar = 0;
                break;
                case ALLEGRO_KEY_ENTER:
                teclaEnter = 0;
                break;
            }

        }
        //****************************************************************************
        //se o evento for uma tecla de texto...
        //https://www.rafaeltoledo.net/tutorial-allegro-5-14-entrada-de-texto/
        //****************************************************************************
        else if (evento.type == ALLEGRO_EVENT_KEY_CHAR){
            caracter = (char)evento.keyboard.unichar;
            if((caracter >= 97)&&(caracter <= 122))caracter-=32;
            if((isalnum((int)caracter))||(caracter==8)||(caracter==32))caracterPendente = 1;
        }

        //****************************************************************************
        // Se o evento foi de movimentação do mouse
        //****************************************************************************
        if (evento.type == ALLEGRO_EVENT_MOUSE_AXES){
            posXMouse = evento.mouse.x/red_x;
            posYMouse = evento.mouse.y/red_y;
        }

        // Ou se o evento foi um clique do mouse
        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
            posXMouseClick = evento.mouse.x/red_x;
            posYMouseClick = evento.mouse.y/red_y;
            printf("\nX: %i",posXMouseClick);
            printf("\nY: %i\n\n",posYMouseClick);
        }

        //****************************************************************************
        //se o evento for Fechar Janela...
        //****************************************************************************
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                sair=1;
        }

        if(sair){
           int resp = al_show_native_message_box(janela,"Fechar","Deseja mesmo sair ???\nSentirei saudades...  :=( ","",NULL,ALLEGRO_MESSAGEBOX_YES_NO);
            if (resp)sair=1;
            else sair=0;
        }
    }//Fecha while(sair)

    // Finaliza o timer, janela e fila_eventos
    al_destroy_timer(timer);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);

    return 0;
}//Fecha void main()
void resetJogo(){
    int i;
    if (cenarioClick == 1){
        backGround.objetoBitmap = al_load_bitmap("img/back/1920/cenario/backgroundEspaco.jpg");
        backGroundExplo.objetoBitmap = al_load_bitmap("img/back/1920/cenario/backgroundEspacoExplo.jpg");
    }else if (cenarioClick ==2){
        backGround.objetoBitmap = al_load_bitmap("img/back/1920/cenario/backgroundCidade.jpg");
        backGroundExplo.objetoBitmap = al_load_bitmap("img/back/1920/cenario/backgroundCidadeExplo.jpg");
    }
    score = 0;
    somaScore = 0;
    tempJogo=0;
    vidas = 3;
    posColisao = 0;
    estadoNave = 0;
    nave.objetoBitmap=naveImagem[tipoNave][estadoNave];
    disparo = 0;
    posColisao = 0;
    limitTiro = 0;

    exploObj.temp=999;
    exploObj2.temp=999;
    exploObj3.temp=999;
    exploObj4.temp=999;

    exploObj.ativo=0;
    exploObj2.ativo=0;
    exploObj3.ativo=0;
    exploObj4.ativo=0;

    vida1.ativo = 1;
    vida2.ativo = 1;
    vida3.ativo = 1;

    ultimaChance.ativo = 0;
    ultimaChance.temp = 0;

    for(i=0;i<NUM_TIROS;i++){
        tiro[i].ativo=false;
    }

    //Declara Dimensoes dos Objetos
    nave.altura = al_get_bitmap_height(nave.objetoBitmap);
    nave.largura = al_get_bitmap_width(nave.objetoBitmap)-50;
    obst.largura = al_get_bitmap_width(obst.objetoBitmap);
    obst.altura = al_get_bitmap_height(obst.objetoBitmap);
    obst2.largura = al_get_bitmap_width(obst2.objetoBitmap);
    obst2.altura = al_get_bitmap_height(obst2.objetoBitmap);
    obst3.largura = al_get_bitmap_width(obst3.objetoBitmap);
    obst3.altura = al_get_bitmap_height(obst3.objetoBitmap);
    obst4.largura = al_get_bitmap_width(obst4.objetoBitmap);
    obst4.altura = al_get_bitmap_height(obst4.objetoBitmap);

    for(i=0;i<NUM_TIROS;i++){
        tiro[i].largura = al_get_bitmap_width(tiro[i].objetoBitmap);
        tiro[i].altura = al_get_bitmap_height(tiro[i].objetoBitmap);
    }

    plusTime.largura = al_get_bitmap_width(plusTime.objetoBitmap);
    plusTime.altura = al_get_bitmap_height(plusTime.objetoBitmap);
    plusScore.largura = al_get_bitmap_width(plusScore.objetoBitmap);
    plusScore.altura = al_get_bitmap_height(plusScore.objetoBitmap);

    explo.largura = al_get_bitmap_width(explo.objetoBitmap);
    explo.altura = al_get_bitmap_height(explo.objetoBitmap);

    vilao.largura = al_get_bitmap_width(vilao.objetoBitmap);
    vilao.altura = al_get_bitmap_height(vilao.objetoBitmap);

    //Declara e inicializa Variaveis com a posição X dos objetos
    backGround.posX=0;
    nave.posX = 100;
    obst.posX = backGround.posX + TELA_LARGURA;
    obst2.posX = backGround.posX + TELA_LARGURA;
    obst3.posX = backGround.posX + TELA_LARGURA;
    obst4.posX = TELA_LARGURA - obst4.largura;
    explo.posX = 4000;
    vilao.posX = 2500;
    vida1.posX = 1600;
    vida2.posX = 1700;
    vida3.posX = 1800;

    plusTime.posX = TELA_LARGURA+1500;
    plusScore.posX = TELA_LARGURA+1500;

    //Declara e inicializa Variaveis com a posição Y dos objetos
    backGround.posY=0;
    obst.posY = 0;
    obst2.posY = 358;
    obst3.posY = 718;
    obst4.posY = 0 ;
    explo.posY = 4000;
    vilao.posY = 100;
    vida1.posY = 10;
    vida2.posY = 10;
    vida3.posY = 10;

    //Declara e inicializa Variaveis com a velocidade dos objetos
    nave.vel= 10;
    for(i=0;i<NUM_TIROS;i++){
        tiro[i].vel = 15;
    }

    obst.vel  = rand()%9 + 5;
    obst2.vel = rand()%9 + 5;
    obst3.vel = rand()%9 + 5;
    obst4.vel = rand()%9 + 5;
    velDesloc = rand()%5 + 6;
    vilao.vel = 4;

    //Declara e inicializa Variavel para contagem de tempo
    tempExploNave = 999;
    posColisao = 0;
    piscaNave=0;
}
bool colisao(Objeto obj1, Objeto obj2){
    bool ok = false;
    if((obj1.ativo)&&(obj2.ativo)){
        int posX1Obj1 = obj1.posX + obj1.largura + obj1.x1Colisao;
        int posY1Obj1 = obj1.posY + obj1.altura + obj1.y1Colisao;
        int posX1Obj2 = obj2.posX + obj2.largura + obj2.x1Colisao;
        int posY1Obj2 = obj2.posY + obj2.altura + obj2.y1Colisao;

        if ( ( ( obj1.posY+obj1.yColisao>=(obj2.posY+obj2.yColisao+TOLERANCIA))&&(obj1.posY+obj1.yColisao<=(posY1Obj2-TOLERANCIA))) || ((posY1Obj1>=(obj2.posY+obj2.yColisao+TOLERANCIA))&&(posY1Obj1<=(posY1Obj2-TOLERANCIA)))){
            if ( ( ( obj1.posX+obj1.xColisao>=(obj2.posX+obj2.xColisao+TOLERANCIA))&&(obj1.posX+obj1.xColisao<=(posX1Obj2-TOLERANCIA))) || ((posX1Obj1>=(obj2.posX+obj2.xColisao+TOLERANCIA))&&(posX1Obj1<=(posX1Obj2-TOLERANCIA)))){
                ok = true;
            }
        }
    }
    return ok;
}
int mousePosicao(ALLEGRO_BITMAP *botao, int posXBotao, int posYBotao){
    int posicao=0;
    if((posXMouse>=posXBotao)&&(posXMouse<=(posXBotao + al_get_bitmap_width(botao)))){
        if((posYMouse>=posYBotao)&&(posYMouse<=(posYBotao + al_get_bitmap_height(botao)))){
            posicao=1;
        }else{
            posicao=0;
        }
    }else{
        posicao=0;
    }
    return posicao;
}
int mouseClick(ALLEGRO_BITMAP *botao, int posXBotao, int posYBotao){
    int click=0;
    if((posXMouseClick>=posXBotao)&&(posXMouseClick<=(posXBotao + al_get_bitmap_width(botao)))){
        if((posYMouseClick>=posYBotao)&&(posYMouseClick<=(posYBotao + al_get_bitmap_height(botao)))){
            click=1;
            posXMouseClick=4000;
            posYMouseClick=4000;
        }else{
            click=0;
        }
    }else{
        click=0;
    }

    return click;
}
void loadBitmap(){

    // CARREGA BITMAPS
    backGround.objetoBitmap =            al_load_bitmap("img/back/1920/cenario/backGroundEspaco.jpg");
    backGroundCidade.objetoBitmap =      al_load_bitmap("img/back/1920/cenario/backGroundCidade.jpg");
    backGroundMenu.objetoBitmap =        al_load_bitmap("img/back/1920/menuInicial/menuInicial1920.jpg");
    backGroundEscolha.objetoBitmap =     al_load_bitmap("img/back/1920/telaEscolha/telaEscolha1920.jpg");
    backGroundPause.objetoBitmap =       al_load_bitmap("img/back/1920/telaPause/telaPause.jpg");
    backGroundRanking.objetoBitmap =     al_load_bitmap("img/back/1920/telaRanking/telaRanking.jpg");
    backGroundFimTempo.objetoBitmap =    al_load_bitmap("img/back/1920/telaFimTempo/TELA_FimTempo.jpg");
    naveImagem[0][0] =                   al_load_bitmap("img/naves/carro.png");
    naveImagem[0][1] =                   al_load_bitmap("img/naves/carro.png");
    naveImagem[0][2] =                   al_load_bitmap("img/naves/carro.png");
    naveImagem[1][0] =                   al_load_bitmap("img/naves/carro.png");
    naveImagem[1][1] =                   al_load_bitmap("img/naves/carro.png");
    naveImagem[1][2] =                   al_load_bitmap("img/naves/carro.png");
    naveImagem[2][0] =                   al_load_bitmap("img/naves/carro.png");
    naveImagem[2][1] =                   al_load_bitmap("img/naves/carro.png");
    naveImagem[2][2] =                   al_load_bitmap("img/naves/carro.png");
    btNaveZero.objetoBitmap =            al_load_bitmap("img/back/1920/telaEscolha/botaoInversoNaveZero.png");
    btNaveUm.objetoBitmap =              al_load_bitmap("img/back/1920/telaEscolha/botaoInversoNaveUm.png");
    btNaveDois.objetoBitmap =            al_load_bitmap("img/back/1920/telaEscolha/botaoInversoNaveDois.png");
    btCenarioEspaco.objetoBitmap =       al_load_bitmap("img/back/1920/telaEscolha/botaoInversoCenarioEspaco.png");
    btCenarioCidade.objetoBitmap =       al_load_bitmap("img/back/1920/telaEscolha/botaoInversoCenarioCidade.png");
    btPlayJogo.objetoBitmap =            al_load_bitmap("img/back/1920/telaEscolha/botaoInversoPlayJogo.png");
    obst.objetoBitmap =                  al_load_bitmap("img/img_novo/tunel_ajuste.png");
    obst2.objetoBitmap =                 al_load_bitmap("img/img_novo/tunel_ajuste.png");
    obst3.objetoBitmap =                 al_load_bitmap("img/img_novo/tunel_ajuste.png");
    obst4.objetoBitmap =                 al_load_bitmap("img/img_novo/script.png");
    explo.objetoBitmap =                 al_load_bitmap("img/explo2.png");
    exploObj.objetoBitmap =              al_load_bitmap("img/exploNave.png");
    exploObj2.objetoBitmap =             al_load_bitmap("img/exploNave.png");
    exploObj3.objetoBitmap =             al_load_bitmap("img/exploNave.png");
    exploObj4.objetoBitmap =             al_load_bitmap("img/exploNave.png");
    vilao.objetoBitmap =                 al_load_bitmap("img/planetas/planetaUm.png");
    btPlay.objetoBitmap =                al_load_bitmap("img/back/1920/menuInicial/botaoInversoPlay.png");
    btExit.objetoBitmap =                al_load_bitmap("img/back/1920/menuInicial/botaoInversoExit.png");
    btPlayAgain.objetoBitmap =           al_load_bitmap("img/back/1920/telaRanking/botaoInversoPlayAgain.png");
    btQuit.objetoBitmap =                al_load_bitmap("img/back/1920/telaRanking/botaoInversoQuit.png");
    btPlayPause.objetoBitmap =           al_load_bitmap("img/back/1920/telaPause/botaoInversoPlayPause.png");
    btExitPause.objetoBitmap =           al_load_bitmap("img/back/1920/telaPause/botaoInversoExitPause.png");
    vida1.objetoBitmap =                 al_load_bitmap("img/vidasmall.png");
    vida2.objetoBitmap =                 al_load_bitmap("img/vidasmall.png");
    vida3.objetoBitmap =                 al_load_bitmap("img/vidasmall.png");
    plusTime.objetoBitmap =              al_load_bitmap("img/plusTime.png");
    plusScore.objetoBitmap =             al_load_bitmap("img/plusScore.png");

    for(i=0;i<NUM_TIROS;i++){
        tiro[i].objetoBitmap =           al_load_bitmap("img/tiroUm.png");
    }

    // CARREGA FONTES TTF
    textTime.objetoFont =              al_load_font("fonte/alarm_clock/alarm_clock.ttf", 80, ALLEGRO_TTF_NO_KERNING);
    textRank.objetoFont =              al_load_font("fonte/alarm_clock/alarm_clock.ttf", 40, ALLEGRO_TTF_NO_KERNING);
    textPlayer.objetoFont =            al_load_font("fonte/alarm_clock/alarm_clock.ttf", 90, ALLEGRO_TTF_NO_KERNING);
    textScoreJogo.objetoFont =         al_load_font("fonte/alarm_clock/alarm_clock.ttf", 80, ALLEGRO_TTF_NO_KERNING);
    ultimaChance.objetoFont =          al_load_font("fonte/alarm_clock/alarm_clock.ttf", 80, ALLEGRO_TTF_NO_KERNING);


}
void telaMenu(){//----------------------------------------------------FUNCAO RESPONSALVEL PELO MENU
     al_draw_bitmap(btPlay.objetoBitmap, btPlay.posX, btPlay.posY, 0);
     al_draw_bitmap(btExit.objetoBitmap, btExit.posX, btExit.posY, 0);
     al_draw_bitmap(backGroundMenu.objetoBitmap, 0, 0, 0);

     if(mouseClick(btPlay.objetoBitmap, btPlay.posX, btPlay.posY)){
        telaAtual=TELA_ESCOLHA;
     }else if(mouseClick(btExit.objetoBitmap,btExit.posX,btExit.posY)){
         sair=1;
     }

     if(mousePosicao(btPlay.objetoBitmap, btPlay.posX, btPlay.posY)){
        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
        al_draw_bitmap(btPlay.objetoBitmap, btPlay.posX, btPlay.posY, 0);
     }else if(mousePosicao(btExit.objetoBitmap,btExit.posX,btExit.posY)){
        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
        al_draw_bitmap(btExit.objetoBitmap, btExit.posX, btExit.posY, 0);
     }else{
        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
     }

}
void telaEscolha(){//----------------------------------------------------FUNCAO RESPONSALVEL PELO MENU
     //Monta a telaEscolha
     al_draw_bitmap(backGroundEscolha.objetoBitmap, 0, 0, 0);
     al_draw_textf(textPlayer.objetoFont, al_map_rgb(255, 255, 0), 980, 631, ALLEGRO_ALIGN_CENTER, "%s",nomePlayer);

     if(indexNome==0){
        strcpy(nomePlayer,"SEU NICK");
     }
     if(caracterPendente){
        if(indexNome<0){
            indexNome = 0;
            strcpy(nomePlayer," ");
        }
        if(teclaApagar){
            indexNome--;
            caracter=' ';
        }
        if(indexNome<TAMANHO_NOME_PLAYER-1){
            nomePlayer[indexNome]=caracter;
            indexNome++;
            nomePlayer[indexNome] = '\0';
        }
        if(teclaApagar){
            indexNome--;
        }
        caracterPendente=0;
     }

     //Funcoes do mouse
     if(mousePosicao(btNaveZero.objetoBitmap, btNaveZero.posX, btNaveZero.posY)){
        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
        al_draw_bitmap(btNaveZero.objetoBitmap,btNaveZero.posX, btNaveZero.posY, 0);
     }else if(mousePosicao(btNaveUm.objetoBitmap, btNaveUm.posX, btNaveUm.posY)){
        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
        al_draw_bitmap(btNaveUm.objetoBitmap,btNaveUm.posX, btNaveUm.posY, 0);
     }else if(mousePosicao(btNaveDois.objetoBitmap, btNaveDois.posX, btNaveDois.posY)){
        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
        al_draw_bitmap(btNaveDois.objetoBitmap,btNaveDois.posX, btNaveDois.posY, 0);
     }else if(mousePosicao(btCenarioEspaco.objetoBitmap,btCenarioEspaco.posX, btCenarioEspaco.posY)){
        al_draw_bitmap(btCenarioEspaco.objetoBitmap, btCenarioEspaco.posX, btCenarioEspaco.posY, 0);
        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
     }else if(mousePosicao(btCenarioCidade.objetoBitmap, btCenarioCidade.posX, btCenarioCidade.posY)){
        al_draw_bitmap(btCenarioCidade.objetoBitmap, btCenarioCidade.posX, btCenarioCidade.posY,0);
        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
     }else if (mousePosicao(btPlayJogo.objetoBitmap, btPlayJogo.posX, btPlayJogo.posY)){
        al_draw_bitmap(btPlayJogo.objetoBitmap, btPlayJogo.posX, btPlayJogo.posY, 0);
        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
     } else{
        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
     }


     if(mouseClick(btNaveZero.objetoBitmap, btNaveZero.posX, btNaveZero.posY)){
        tipoNave=0;
        naveClick = 1;
     }else if(mouseClick(btNaveUm.objetoBitmap, btNaveUm.posX, btNaveUm.posY)){
        tipoNave=1;
        naveClick = 2;
     }else if(mouseClick(btNaveDois.objetoBitmap, btNaveDois.posX, btNaveDois.posY)){
        tipoNave=2;
        naveClick = 3;
     }else if(mouseClick(btCenarioEspaco.objetoBitmap, btCenarioEspaco.posX, btCenarioEspaco.posY)){
        cenarioClick = 1;
        rTime = 255;
        gTime = 255;
        bTime = 0;
        vilao.ativo=true;
     }else if(mouseClick(btCenarioCidade.objetoBitmap, btCenarioCidade.posX, btCenarioCidade.posY)){
        cenarioClick = 2;
        vilao.ativo=false;
        rTime = 255;
        gTime = 20;
        bTime = 147;

     }else if(mouseClick(btPlayJogo.objetoBitmap, btPlayJogo.posX, btPlayJogo.posY)){
        if ((cenarioClick == 1)||(cenarioClick == 2)){
            if(strcmp(nomePlayer,"SEU NICK")){
                resetJogo();
                telaAtual = TELA_JOGO;
            }else al_show_native_message_box(janela,"HEY INDIGENTE","QUEM E VOCE AFINAL?\nDIGITE SEU NICK DE PERDEDOR!!!","",NULL,ALLEGRO_MESSAGEBOX_WARN);
        }else al_show_native_message_box(janela,"ACORDE ! ! !","DEIXE DE PREGUICA,\nESCOLHA UM CENARIO!","",NULL,ALLEGRO_MESSAGEBOX_WARN);
     }

     if (naveClick == 1){
        al_draw_bitmap(btNaveZero.objetoBitmap, btNaveZero.posX, btNaveZero.posY,0);
     } else if (naveClick == 2) {
        al_draw_bitmap(btNaveUm.objetoBitmap, btNaveUm.posX, btNaveUm.posY,0);
     } else if (naveClick == 3){
        al_draw_bitmap(btNaveDois.objetoBitmap, btNaveDois.posX, btNaveDois.posY,0);
     }
     if (cenarioClick == 1){
        al_draw_bitmap(btCenarioEspaco.objetoBitmap, btCenarioEspaco.posX, btCenarioEspaco.posY,0);
     } else if (cenarioClick == 2){
        al_draw_bitmap(btCenarioCidade.objetoBitmap, btCenarioCidade.posX, btCenarioCidade.posY,0);
     }

}
void telaPause(){//---------------------------------------------------FUNCAO RESPONSALVEL PELO PAUSE
    al_draw_bitmap(backGroundPause.objetoBitmap, 0, 0, 0);



    if(mouseClick(btPlayPause.objetoBitmap, btPlayPause.posX, btPlayPause.posY)){
        telaAtual = TELA_JOGO;
     }else if(mouseClick(btExitPause.objetoBitmap,btExitPause.posX,btExitPause.posY)){
         telaAtual = TELA_RANKING;
     }

     if(mousePosicao(btPlayPause.objetoBitmap, btPlayPause.posX, btPlayPause.posY)){
        al_draw_bitmap(btPlayPause.objetoBitmap, btPlayPause.posX, btPlayPause.posY, 0);
        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
     }else if(mousePosicao(btExitPause.objetoBitmap,btExitPause.posX,btExitPause.posY)){
        al_draw_bitmap(btExitPause.objetoBitmap, btExitPause.posX, btExitPause.posY, 0);
        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
     }else{
        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
     }

}
int abreArquivoDisco(char rankingPlayersArq[][50], int numJogadores, int scoreArq[]){
    FILE *arq;
    char texto[200];
    int result,i;

    printf("\nProcurando jogos salvos em Disco...");
    arq = fopen("savedGames/Jogos_Salvos.txt","r");
    if(arq == NULL) {
        printf("\nArquivo nao encontrado.\n    Criando novo arquivo...");
        //------------------------------------------------------------------------------------------
        for(i=0;i<NUM_JOGADORES;i++){
            strcpy(&rankingPlayersArq[i][0],"[VAZIO]");
            scoreArq[i]=0;
        }
        arq = fopen("savedGames/Jogos_Salvos.txt","w");
        if(arq == NULL) {
            printf("Problemas na ABERTURA do arquivo linha 261\n");
        }
        result = fprintf(arq,"Posicao:\tPontuacao:\tNome:\n");
        if(result < 0)printf("Erro na escrita\n");
        for(i=0;i<NUM_JOGADORES;i++){
            result = fprintf(arq,"%i º\t\t",i+1);
            result = fprintf(arq,"%i\t\t",scoreArq[i]);
            result = fprintf(arq,"%s\n",rankingPlayersArq[i]);
            if(result < 0)printf("Erro na escrita\n");
        }
        fclose(arq);

        arq = fopen("savedGames/Jogos_Salvos.txt","r");
        if(arq == NULL) printf("\nErro ao abrir novo arquivo...");
        else printf("\nNovo Arquivo Criado com sucesso!!!");

        //------------------------------------------------------------------------------------------
    }
    fgets(texto,200,arq);//Le cabecaolho e descarta

    for(i=0;i<NUM_JOGADORES;i++){
        fscanf(arq," %s", texto); //Lê a posicao e descarta
        fscanf(arq," %s", texto); //Lê o "º" e descarta
        fscanf(arq," %i", &scoreArq[i]); //Lê a pontuacao e armazena
        fgets(&rankingPlayersArq[i],50,arq);  //Lê o Nome do Player e armazena
    }

    fclose(arq);

    return 1;
}
int SalvaArquivoDisco(char rankingPlayersArq[][50], int numJogadores, int scoreArq[]){

    FILE *arq;
    int result,i;

    arq = fopen("savedGames/Jogos_Salvos.txt","w");
    if(arq == NULL) {
        printf("Problemas na ABERTURA do arquivo\n");
    }
    printf("\nSalvando os dados...");
    result = fprintf(arq,"Posicao:\tPontuacao:\tNome:\n");
    if(result < 0)printf("Erro na escrita\n");
    for(i=0;i<NUM_JOGADORES;i++){
        result = fprintf(arq,"%i º\t\t",i+1);
        result = fprintf(arq,"%i",scoreArq[i]);
        result = fprintf(arq,"%s\n",rankingPlayersArq[i]);
    }
    if(result < 0)printf("Erro na escrita\n");
    else printf("\n   Dados salvos com Sucesso!!!...");
    fclose(arq);

    return 1;
}
void ordenaRanking(int *ranking[], char nomeJogadores[][50]){
    int i,j,aux;
    char auxNome[50];

    for(i=1;i<NUM_JOGADORES;i++){
        if(ranking[i]>ranking[i-1]){
            aux = ranking[i];
            strcpy(auxNome,nomeJogadores[i]);
            ranking[i] = ranking [i-1];
            strcpy(nomeJogadores[i],nomeJogadores[i-1]);
            ranking[i-1] = aux;
            strcpy(nomeJogadores[i-1],auxNome);

            for(j=i-1;j>0;j--){
                if(ranking[j]>ranking[j-1]){
                    aux = ranking[j];
                    strcpy(auxNome,nomeJogadores[j]);
                    ranking[j] = ranking [j-1];
                    strcpy(nomeJogadores[j],nomeJogadores[j-1]);
                    ranking[j-1] = aux;
                    strcpy(nomeJogadores[j-1],auxNome);
                }else{
                    break;
                }
            }
        }
    }


}
void telaRanking(){//-------------------------------------------------FUNCAO RESPONSALVEL PELO RANKING
    char txtF[TAMANHO_NOME_PLAYER]; // Recebe Texto Formatado

    if(!jogoSalvo){
        // ABRE OU CRIA UM AQUIVO E COPIA VALORES SALVOS PARA OS ARRAYS DE NOME E PONTUACAO
        jogoSalvo = abreArquivoDisco(rankingPlayers,NUM_JOGADORES,rankingScore);
        // INSERE OS DADOS DO ÚLTIMO JOGO REALIZADO NO PRIMEIRO ESPAÇO DISPONIVEL DO ARRAY
        for(i=0;i<NUM_JOGADORES;i++){
            if(rankingScore[i]==0){
                strcpy(&rankingPlayers[i],"\t\t");
                strcat(&rankingPlayers[i],&nomePlayer);
                strcat(&rankingPlayers[i],"\n");
                rankingScore[i] = somaScore*progressao;
                break;
            }
        }
        //SE NÃO HÁ ESPAÇO DISPONÍVEL NOS ARRAYS, INSERE DADOS DO ÚLTIMO JOGO NA ÚLTIMA LINHA DOS ARRAYS
        if(i>=NUM_JOGADORES){
            strcpy(&rankingPlayers[NUM_JOGADORES-1],"\t\t");
            strcat(&rankingPlayers[NUM_JOGADORES-1],&nomePlayer);
            strcat(&rankingPlayers[NUM_JOGADORES-1],"\n");
            rankingScore[NUM_JOGADORES-1] = somaScore*progressao;
        }

        //ORDENA OS ARRAYS DA MAIOR PARA A MENOR PONTUAÇÃO (DE CIMA PARA BAIXO RESPECTIVAMENTE)
        ordenaRanking(rankingScore, rankingPlayers);

        //SALVA ARRAYS ATUALIZADOS NO ARQUIVO
        jogoSalvo = SalvaArquivoDisco(rankingPlayers,NUM_JOGADORES,rankingScore);
    }

    al_draw_bitmap(backGroundRanking.objetoBitmap, 0, 0, 0);

    //SCORE FINAL
    al_draw_textf(textScoreJogo.objetoFont, al_map_rgb(255, 255, 0), 270, 65, ALLEGRO_ALIGN_LEFT, "%i",somaScore*progressao);
    //RANKING PLAYERS
    int posYTextRanking = 180;
    for(i=0;i<NUM_JOGADORES;i++){
        strcpy(txtF, &rankingPlayers[i][2]);
        txtF[strlen(txtF)-1]='\0';
        txtF[TAMANHO_NOME_PLAYER-1]='\0';
        al_draw_text(textRank.objetoFont, al_map_rgb(255, 255, 0), 270, posYTextRanking, ALLEGRO_ALIGN_LEFT, txtF);
        al_draw_textf(textRank.objetoFont, al_map_rgb(255, 255, 0), 620, posYTextRanking, ALLEGRO_ALIGN_RIGHT, "%i", rankingScore[i]);
        posYTextRanking += 60;
    }

    if(mouseClick(btPlayAgain.objetoBitmap, btPlayAgain.posX, btPlayAgain.posY)){
        naveClick = 0;
        cenarioClick = 0;
        jogoSalvo = 0;
        strcpy(nomePlayer,"SEU NICK");
        indexNome = -1;
        caracterPendente=0;
        caracter='\0';
        teclaApagar = 0;
        resetJogo();
        telaAtual=TELA_ESCOLHA;
     }else if(mouseClick(btQuit.objetoBitmap,btQuit.posX,btQuit.posY)){
         jogoSalvo = 0;
         sair=1;
     }

     if(mousePosicao(btPlayAgain.objetoBitmap, btPlayAgain.posX, btPlayAgain.posY)){
        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
        al_draw_bitmap(btPlayAgain.objetoBitmap, btPlayAgain.posX, btPlayAgain.posY, 0);
     }else if(mousePosicao(btQuit.objetoBitmap,btQuit.posX,btQuit.posY)){
        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
        al_draw_bitmap(btQuit.objetoBitmap, btQuit.posX, btQuit.posY, 0);
     }else{
        al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
     }

}
void drawTelaJogo (){

    //------------------------------------------------------------Desenha/Atualiza objetos na tela
    if(tempExploNave>=TEMPO_EXPLOSAO)al_draw_bitmap(backGround.objetoBitmap, backGround.posX, backGround.posY, 0);
    else al_draw_bitmap(backGroundExplo.objetoBitmap, backGround.posX, backGround.posY, 0);
    if(vilao.ativo)al_draw_bitmap(vilao.objetoBitmap, vilao.posX, vilao.posY, 0);

    for(i=0;i<NUM_TIROS;i++){
        if(tiro[i].ativo)al_draw_bitmap(tiro[i].objetoBitmap, tiro[i].posX, tiro[i].posY, 0);
    }
    if(nave.ativo)al_draw_rotated_bitmap(nave.objetoBitmap, nave.largura/2, nave.altura/2, nave.posX, nave.posY,nave.ang,0);

    if(obst.ativo)al_draw_bitmap(obst.objetoBitmap, obst.posX, obst.posY, 0);
    if(obst2.ativo)al_draw_bitmap(obst2.objetoBitmap, obst2.posX, obst2.posY, 0);
    if(obst3.ativo)al_draw_bitmap(obst3.objetoBitmap, obst3.posX, obst3.posY, 0);
    if(obst4.ativo)al_draw_bitmap(obst4.objetoBitmap, obst4.posX, obst4.posY, 0);
    if(explo.ativo)al_draw_bitmap(explo.objetoBitmap, explo.posX, explo.posY, 0);
    if(exploObj.ativo)al_draw_bitmap(exploObj.objetoBitmap, exploObj.posX, exploObj.posY, 0);
    if(exploObj2.ativo)al_draw_bitmap(exploObj2.objetoBitmap, exploObj2.posX, exploObj2.posY, 0);
    if(exploObj3.ativo)al_draw_bitmap(exploObj3.objetoBitmap, exploObj3.posX, exploObj3.posY, 0);
    if(exploObj4.ativo)al_draw_bitmap(exploObj4.objetoBitmap, exploObj4.posX, exploObj4.posY, 0);
    if(vida1.ativo)al_draw_bitmap(vida1.objetoBitmap,vida1.posX,vida1.posY,0);
    if(vida2.ativo)al_draw_bitmap(vida2.objetoBitmap,vida2.posX,vida2.posY,0);
    if(vida3.ativo)al_draw_bitmap(vida3.objetoBitmap,vida3.posX,vida3.posY,0);
    if(plusTime.ativo)al_draw_rotated_bitmap(plusTime.objetoBitmap, (al_get_bitmap_width(plusTime.objetoBitmap))/4, (al_get_bitmap_height(plusTime.objetoBitmap))/4, plusTime.posX, plusTime.posY,plusTime.ang,0);
    if(plusScore.ativo)al_draw_rotated_bitmap(plusScore.objetoBitmap, (al_get_bitmap_width(plusScore.objetoBitmap))/4, (al_get_bitmap_height(plusScore.objetoBitmap))/5, plusScore.posX, plusScore.posY,plusScore.ang,0);
    if(textScoreJogo.ativo)al_draw_textf(textScoreJogo.objetoFont, al_map_rgb(rTime, gTime, bTime), 500, 10, ALLEGRO_ALIGN_LEFT, "SCORE: %i",somaScore*progressao);
    if(textTime.ativo)al_draw_textf(textTime.objetoFont, al_map_rgb(rTime, gTime, bTime), 20 , 10, ALLEGRO_ALIGN_LEFT, "TIME: %i",TEMPO_JOGO_SEGUNDOS - (tempJogo/FPS));
    if(ultimaChance.ativo)al_draw_textf(ultimaChance.objetoFont, al_map_rgb(rTime, gTime, bTime), ultimaChance.posX-350, ultimaChance.posY, ALLEGRO_ALIGN_LEFT, ultChTexto);
}
void telaJogo(){//----------------------------------------------------FUNCAO RESPONSALVEL PELO JOGO
    int i,r;

    if(posColisao<120){//---------------------------------------------PAUSA APOS OCORRENCIA DE COLISAO(JOGO INATIVO)
        //------------------------------------------------------------MANTEM BACKGROUND EM MOVIMENTO
        backGround.posX -= nave.vel*0.7;
        if(backGround.posX<=(TELA_LARGURA*-2)){
            backGround.posX=0;
        }
          //Posicionamento Vilao
        vilao.posX -= vilao.vel+nave.vel;
        if (vilao.posX<=-(vilao.largura-10)){
            vilao.posY = (rand()% (TELA_ALTURA-vilao.altura)) + 0;
            vilao.posX = (rand()% TELA_LARGURA*1.5) + 2000;
        }
        //------------------------------------------------------------PISCA NAVE ENQUANTO AGUARDA RETOMADA DO JOGO
        piscaNave++;
        if(piscaNave>=(120/20)){
            if(nave.posY!=500){
                nave.posY=500;
                nave.posX=150;
                nave.ang= 2*ALLEGRO_PI;
            }else{
                nave.posY=4800;
            }
            piscaNave=0;
        }
        posColisao++;
    }else if(posColisao==120){//-------------------------------------RETOMADA DO JOGO APOS COLISAO
        posColisao=4000;
        nave.posX = 150;
        nave.posY = 500;

    }else{//----------------------------------------------------------ROTINA DO JOGO ATIVO

        //------------------------------------------------------------TRATAMENTO DAS POSIÇÕES E VELOCIDADES(TODOS OS OBJETOS)
        //Posicionamento do background (VELOCIDADE DA NAVE)
        backGround.posX -= nave.vel*0.7;
        if(backGround.posX<=(TELA_LARGURA*-2)){
            backGround.posX=0;
            r = 0;
        }
         //Posicionamento Vilao
        vilao.posX -= vilao.vel+nave.vel;
        if (vilao.posX<=-(al_get_bitmap_width(vilao.objetoBitmap)+10)){
            vilao.posY = (rand()% (TELA_ALTURA-al_get_bitmap_height(vilao.objetoBitmap))) + 0;
            vilao.posX = (rand()% TELA_LARGURA*1.5) + 2000;
        }
        //Posicionamento PlusTime
        plusTime.ang += (0.0349066)*2;
        plusTime.posX -= (plusTime.vel+nave.vel);
        if (plusTime.posX <= -50){
            plusTime.posY = (rand()% TELA_ALTURA-100) + 100;
            plusTime.posX = (rand()% TELA_LARGURA*1.5) + 2000;
            plusTime.vel = (rand()% 2) + 1;
        }
        //Posicionamento PlusScore
        plusScore.ang += (0.0349066)*2;
        plusScore.posX -= (plusScore.vel+nave.vel);
        if (plusScore.posX <= -50){
            plusScore.posY = (rand()% TELA_ALTURA-100) + 100;
            plusScore.posX = (rand()% TELA_LARGURA*1.5) + 2000;
            plusScore.vel = (rand()% 2) + 1;
        }
        //Posicionamento do obst
        obst.posX = backGround.posX + TELA_LARGURA;

        //Posicionamento do obst2
        obst2.posX = backGround.posX + TELA_LARGURA;

        //Posicionamento do obst3
        obst3.posX = backGround.posX + TELA_LARGURA;



        //------------------------------------------------------------TRATAMENTO DAS COLISÕES
        if (colisao(nave,obst)){
            strcpy(ultChTexto,"TUNEL 1");
            ultimaChance.ativo = true;

        }else if (colisao(nave,obst2)){
            strcpy(ultChTexto,"TUNEL 2");
            ultimaChance.ativo = true;

        }else if (colisao(nave,obst3)){
            strcpy(ultChTexto,"TUNEL 3");
            ultimaChance.ativo = true;

        }else if (colisao(nave,obst4)){
            strcpy(ultChTexto,"TUNEL 4");
            ultimaChance.ativo = true;
        }

        if (colisao(nave,plusTime)){
            tempJogo = tempJogo - 120;
            plusTime.posY = (rand()% 1000) + 100;
            plusTime.posX = (rand()% TELA_LARGURA*1.5) + 2000;
        }
        if (colisao(nave,plusScore)){
            somaScore += 2;
            plusScore.posY = (rand()% 1000) + 100;
            plusScore.posX = (rand()% TELA_LARGURA*1.5) + 2000;
        }

        //------------------------------------------------------------SISTEMA DE VIDAS
            if (vidas<3){
                vida1.ativo=0;
            }
            if (vidas<2){
                vida2.ativo=0;
            }
            if (vidas==0){
               vida3.ativo=0;
               ultimaChance.temp++;
               if(ultimaChance.temp>10){
                    ultimaChance.temp=0;
                    if(ultimaChance.ativo==1)ultimaChance.ativo=0;
                    else ultimaChance.ativo=1;
               }
            }
            if (vidas<0){
                telaAtual = TELA_RANKING;
            }

        //------------------------------------------------------------TRATAMENTO DAS EXPLOSÕES
        if(tempExploNave<TEMPO_EXPLOSAO){
            explo.posX -= nave.vel;
            tempExploNave++;
        }else if(tempExploNave==TEMPO_EXPLOSAO){
            explo.posX = 5000;
            tempExploNave++;
            posColisao=0;
            obst.posX = (rand()% TELA_LARGURA*1.5) + 2000;
            obst.posY = (rand()% 300) + 50;
            obst2.posX = (rand()% TELA_LARGURA*1.5) + 2000;
            obst2.posY = (rand() % 600) + 320;
            obst3.posX = (rand()% TELA_LARGURA*1.5) + 2000;
            obst3.posY = (rand()% 1000) + 680;
        }
        //------------------------------------------------------------CONTROLA TEMPO DE JOGO
        tempJogo++;
        if(tempJogo>TEMPO_JOGO_SEGUNDOS*FPS){
            tempTelaFimTempo = 0;
            telaAtual=TELA_FIM_TEMPO;
        }

        //------------------------------------------------------------LIMITE SUP E INF
        if (nave.posY <= nave.altura/2){
            nave.posY = nave.altura/2;
        }
        if (nave.posY >= TELA_ALTURA-nave.altura/2){
            nave.posY =  TELA_ALTURA-nave.altura/2;
        }

        if (nave.posX <= nave.largura/2){
            nave.posX = nave.largura/2;
        }
        if (nave.posX >= TELA_LARGURA-nave.largura/2){
            nave.posX =  TELA_LARGURA-nave.largura/2;
        }

        //------------------------------------------------------------CONTROLES DO TECLADO
        if(cima){
            nave.posY-= velDesloc;
            nave.ang=(6.10865);
        }else if(baixo){
            nave.posY+= velDesloc;
            nave.ang=(0.174533);
        }else{
            nave.ang= 2*ALLEGRO_PI;
        }
        if(freia){
            nave.posX-= velDesloc;
        }
        if(acelera){
            nave.posX+= velDesloc;
        }

    }
    drawTelaJogo();

}
void telaFimTempo(){
    //Monta a telaFimTempo
     al_draw_bitmap(backGroundFimTempo.objetoBitmap, 0, 0, 0);
     tempTelaFimTempo++;
     if(tempTelaFimTempo>4*FPS){
        telaAtual = TELA_RANKING;
     }
}
void inicializaObjetos(){
    int i;
    backGround.ativo=true;
    backGroundExplo.ativo=true;
    backGroundCidade.ativo=true;
    cenario1.ativo=true;
    cenario2.ativo=true;
    cenario3.ativo=true;
    backGroundMenu.ativo=true;
    backGroundEscolha.ativo=true;
    backGroundRanking.ativo=true;
    backGroundPause.ativo=true;
    backGroundFimTempo.ativo=true;
    nave.ativo=true;

    for(i=0;i<NUM_TIROS;i++){
        tiro[i].ativo=false;
    }

    obst.ativo=true;
    obst2.ativo=true;
    obst3.ativo=true;
    obst4.ativo=true;
    explo.ativo=true;
    exploObj.ativo=true;
    exploObj2.ativo=true;
    exploObj3.ativo=true;
    exploObj4.ativo=true;
    vilao.ativo=true;
    btPlay.ativo=true;
    btExit.ativo=true;
    btPlayAgain.ativo=true;
    btQuit.ativo=true;
    btNaveZero.ativo=true;
    btNaveUm.ativo=true;
    btNaveDois.ativo=true;
    btCenarioEspaco.ativo=true;
    btCenarioCidade.ativo=true;
    btPlayJogo.ativo=true;
    btPlayPause.ativo=true;
    btExitPause.ativo=true;
    vida1.ativo=true;
    vida2.ativo=true;
    vida3.ativo=true;
    plusTime.ativo=true;
    plusScore.ativo=true;
    textTime.ativo=true;
    textRank.ativo=true;
    textScoreJogo.ativo=true;
    textPlayer.ativo=true;
    ultimaChance.ativo=true;



    backGround.posX=0;
    nave.posX = 200;
    nave.xColisao = -90;
    nave.yColisao = -70;
    nave.x1Colisao = -90;
    nave.y1Colisao = -100;

    for(i=0;i<NUM_TIROS;i++){
        tiro[i].posX = 100;
        tiro[i].xColisao = 0;
        tiro[i].yColisao = 0;
        tiro[i].x1Colisao = 0;
        tiro[i].y1Colisao = 0;
    }

    explo.posX = 0;
    obst.posX = backGround.posX + TELA_LARGURA;
    obst.xColisao = 30;
    obst.yColisao = 10;
    obst.x1Colisao = -30;
    obst.y1Colisao = -10;

    obst2.posX = backGround.posX + TELA_LARGURA;
    obst2.xColisao = 30;
    obst2.yColisao = 10;
    obst2.x1Colisao = -30;
    obst2.y1Colisao = -10;

    obst3.posX = backGround.posX + TELA_LARGURA;
    obst3.xColisao = 30;
    obst3.yColisao = 10;
    obst3.x1Colisao = -30;
    obst3.y1Colisao = -10;

    obst4.posX = backGround.posX + TELA_LARGURA;
    obst4.xColisao = 30;
    obst4.yColisao = 10;
    obst4.x1Colisao = -30;
    obst4.y1Colisao = -10;

    vilao.posX = -5000;
    btPlay.posX = 496;
    btExit.posX = 1032;
    btPlayAgain.posX = 807;
    btQuit.posX = 1430;
    btNaveZero.posX = 303;
    btNaveUm.posX = 303;
    btNaveDois.posX = 303;
    btCenarioEspaco.posX = 1503;
    btCenarioCidade.posX = 1515;
    btPlayJogo.posX = 840;
    btPlayPause.posX = 248;
    btExitPause.posX = 1462;
    vida1.posX = 1600;
    vida2.posX = 1700;
    vida3.posX = 1800;
    ultimaChance.posX = 1300;
    textTime.posX = 20;
    plusTime.posX = 4000;
    plusTime.xColisao = 0;
    plusTime.yColisao = 0;
    plusTime.x1Colisao = 0;
    plusTime.y1Colisao = 0;
    plusScore.posX = 4000;
    plusScore.xColisao = 0;
    plusScore.yColisao = 0;
    plusScore.x1Colisao = 0;
    plusScore.y1Colisao = 0;


    backGround.posY=0;
    nave.posY= 500;
    for(i=0;i<NUM_TIROS;i++){
        tiro[i].posY = -1000;
    }

    obst.posY = 1*(TELA_ALTURA/5);
    obst2.posY = 2*(TELA_ALTURA/5);
    obst3.posY = 3*(TELA_ALTURA/5);
    obst4.posY = 4*(TELA_ALTURA/5);
    explo.posY = 3999;
    vilao.posY = 3000;
    btPlay.posY = 560;
    btExit.posY = 560;
    btPlayAgain.posY = 110;
    btQuit.posY = 110;
    btNaveZero.posY = 85;
    btNaveUm.posY = 390;
    btNaveDois.posY = 690;
    btCenarioEspaco.posY = 160;
    btCenarioCidade.posY = 620;
    btPlayJogo.posY = 738;
    btPlayPause.posY = 283;
    btExitPause.posY = 280;
    vida1.posY = 10;
    vida2.posY = 10;
    vida3.posY = 10;
    ultimaChance.posY = 20;
    textTime.posY = 20;
    plusTime.posY = 2000;
    plusScore.posY = 2000;

    nave.ang= 2*ALLEGRO_PI;
    obst.ang= 0.0;
    obst2.ang= 0.0;
    obst3.ang = 0.0;
    obst4.ang = 0.0;
    plusTime.ang = 0.0;
    plusScore.ang = 0.0;

    nave.vel= 10;
    for(i=0;i<NUM_TIROS;i++){
        tiro[i].vel = 15;
        tiro[i].vel = 15;
    }

    obst.vel  = 4;
    obst2.vel = 4;
    obst3.vel = 4;
    obst4.vel = 4;
    vilao.vel = 4;
    plusTime.vel = 4;
    plusScore.vel = 4;
}
