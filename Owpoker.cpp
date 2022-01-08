//-------------------------------------------------------------------
// AMSPoker for Windows 95 - (c) Copyright 1996 by A.Soares
//-------------------------------------------------------------------
#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\dialog.h>
#include <owl\framewin.h>
#include <owl\vbxctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "owpoker.rh"
#include "hands.h"
//-------------------------------------------------------------------
//-------------------------------------------------------------------
class TPokerDialog : public TDialog, public TVbxEventHandler
{
	public:
		TPokerDialog(TWindow* parent,TResId resId);
		~TPokerDialog();
	protected:
		TVbxControl* Card1;
		TVbxControl* Card2;
		TVbxControl* Card3;
		TVbxControl* Card4;
		TVbxControl* Card5;
		TVbxControl* Card6;
		TVbxControl* BRCard;
		TButton* RedButton;
		TButton* BlackButton;
		TButton* RetryButton;
		TButton* BetButton;
		TButton* MaxBetButton;
		TButton* CreditButton;
		TButton* DoubleButton;
		TListBox* PrizeList;
		TListBox* RewardList;
		TStatic* Credits;
		TStatic* Wins;
		BOOL BackOn1;
		BOOL BackOn2;
		BOOL BackOn3;
		BOOL BackOn4;
		BOOL BackOn5;
		BOOL my_flag;
		BOOL end_of_play;
		BOOL ok;
		short number[53];
		int Bet;
	protected:
		void SetupWindow();
		void EvCardMouseDown(VBXEVENT far* event);
		void EvClickCard1(VBXEVENT far* event);
		void EvClickCard2(VBXEVENT far* event);
		void EvClickCard3(VBXEVENT far* event);
		void EvClickCard4(VBXEVENT far* event);
		void EvClickCard5(VBXEVENT far* event);
		void EvClickCard6(VBXEVENT far* event);
		void UpdateCard(TVbxControl* card);
		void CmAbout();
		void ProcessRetryButton();
		void ProcessBetButton();
		void ProcessMaxBetButton();
		void ProcessCreditButton();
		void ProcessDoubleButton();
		void ProcessRedButton();
		void ProcessBlackButton();
		void Verify_Hand(short* nmbr);
	DECLARE_RESPONSE_TABLE(TPokerDialog);
};
//-------------------------------------------------------------------
//-------------------------------------------------------------------
DEFINE_RESPONSE_TABLE2(TPokerDialog, TDialog, TVbxEventHandler)
	EV_VBXEVENTNAME(IDC_MHCARDDECK6,"MouseDown",EvCardMouseDown),
	EV_VBXEVENTNAME(IDC_MHCARDDECK1,"Click",EvClickCard1),
	EV_VBXEVENTNAME(IDC_MHCARDDECK2,"Click",EvClickCard2),
	EV_VBXEVENTNAME(IDC_MHCARDDECK3,"Click",EvClickCard3),
	EV_VBXEVENTNAME(IDC_MHCARDDECK4,"Click",EvClickCard4),
	EV_VBXEVENTNAME(IDC_MHCARDDECK5,"Click",EvClickCard5),
	EV_VBXEVENTNAME(IDC_MHCARDDECK6,"Click",EvClickCard6),
	EV_COMMAND(CM_ABOUT, CmAbout),
	EV_COMMAND(IDRETRY,ProcessRetryButton),
	EV_COMMAND(IDC_BETBUTTON,ProcessBetButton),
	EV_COMMAND(IDC_MAXBETBUTTON,ProcessMaxBetButton),
	EV_COMMAND(IDC_CREDITBUTTON,ProcessCreditButton),
	EV_COMMAND(IDC_DOUBLEBUTTON,ProcessDoubleButton),
	EV_COMMAND(IDC_REDBTN,ProcessRedButton),
	EV_COMMAND(IDC_BLACKBTN,ProcessBlackButton),
END_RESPONSE_TABLE;
//-------------------------------------------------------------------
TPokerDialog::TPokerDialog(TWindow* parent,TResId resId)
	:TDialog(parent,resId)
{
	Card1=new TVbxControl(this,IDC_MHCARDDECK1);
	Card2=new TVbxControl(this,IDC_MHCARDDECK2);
	Card3=new TVbxControl(this,IDC_MHCARDDECK3);
	Card4=new TVbxControl(this,IDC_MHCARDDECK4);
	Card5=new TVbxControl(this,IDC_MHCARDDECK5);
	Card6=new TVbxControl(this,IDC_MHCARDDECK6);
	RetryButton=new TButton(this,IDRETRY);
	BetButton=new TButton(this,IDC_BETBUTTON);
	MaxBetButton=new TButton(this,IDC_MAXBETBUTTON);
	CreditButton=new TButton(this,IDC_CREDITBUTTON);
	DoubleButton=new TButton(this,IDC_DOUBLEBUTTON);
	PrizeList=new TListBox(this,IDC_LISTBOX1);
	RewardList=new TListBox(this,IDC_LISTBOX2);
	Credits=new TStatic(this,IDC_STATIC1);
	Wins=new TStatic(this,IDC_STATIC2);
	RedButton=new TButton(this,IDC_REDBTN);
	BlackButton=new TButton(this,IDC_BLACKBTN);
	BRCard=new TVbxControl(this,IDC_DOUBLECARD);
}
//-------------------------------------------------------------------
TPokerDialog::~TPokerDialog()
{
	delete Card1;
	delete Card2;
	delete Card3;
	delete Card4;
	delete Card5;
	delete Card6;
	delete RetryButton;
	delete BetButton;
	delete MaxBetButton;
	delete CreditButton;
	delete DoubleButton;
	delete PrizeList;
	delete RewardList;
	delete Credits;
	delete Wins;
	delete RedButton;
	delete BlackButton;
	delete BRCard;
}
//-------------------------------------------------------------------
static void SetCredits(TStatic* credits,char* str)
{
	credits->SetText(str);
}
//-------------------------------------------------------------------
static void FillPrizeList(TListBox* list)
{
	list->AddString("Royal Flush");
	list->AddString("Straight Flush");
	list->AddString("4 of a Kind");
	list->AddString("Full House");
	list->AddString("Flush");
	list->AddString("Straight");
	list->AddString("3 of a Kind");
	list->AddString("Two Pairs");
	list->AddString("Pair of Aces");
}
//-------------------------------------------------------------------
static void SetRewards(TListBox* list,int bet)
{
	list->SetItemData(0,500*bet);
	list->SetItemData(1,80*bet);
	list->SetItemData(2,25*bet);
	list->SetItemData(3,10*bet);
	list->SetItemData(4,8*bet);
	list->SetItemData(5,5*bet);
	list->SetItemData(6,3*bet);
	list->SetItemData(7,2*bet);
	list->SetItemData(8,1*bet);
}
//-------------------------------------------------------------------
static void FillRewardList(TListBox* prize,TListBox* reward)
{
char data[6];
char numbertochar[6];

for (int i=0;i<=8;i++)
	{
	sprintf(numbertochar,"%d",prize->GetItemData(i));
	strcpy(data,numbertochar);
	reward->AddString(data);
	}
}
//-------------------------------------------------------------------
void TPokerDialog::SetupWindow()
{
	TDialog::SetupWindow();
	Bet=0;
	RetryButton->EnableWindow(FALSE);
	BetButton->EnableWindow(TRUE);
	MaxBetButton->EnableWindow(TRUE);
	CreditButton->EnableWindow(FALSE);
	DoubleButton->EnableWindow(FALSE);
	SetCredits(Credits,"100");
	FillPrizeList(PrizeList);
	SetRewards(PrizeList,Bet);
	FillRewardList(PrizeList,RewardList);
	randomize();
	my_flag=TRUE;
	end_of_play=TRUE;
	ok=TRUE;
	for (int n=0;n<=52;n++)
		number[n]=0;
}
//-------------------------------------------------------------------
void TPokerDialog::CmAbout()
{
TDialog* aboutdlg;
aboutdlg=new TDialog(this,IDD_ABOUT);
aboutdlg->Execute();
}
//-------------------------------------------------------------------
void TPokerDialog::ProcessBetButton()
{
int currCredits;
char str[7];

	if (Bet<10)
	{
	SoundBet();
	Bet++;
	SetRewards(PrizeList,Bet);
	RewardList->ClearList();
	FillRewardList(PrizeList,RewardList);
	Credits->GetText(str,7);
	currCredits=atoi(str);
	currCredits=currCredits-1;
	itoa(currCredits,str,10);
	Credits->Clear();
	Credits->SetText(str);
	}
	if (Bet==1)
	{
	RetryButton->EnableWindow(TRUE);
	for (int c=0;c<=52;c++)
		number[c]=0;
	}
	if (Bet==10)
	{
	BetButton->EnableWindow(FALSE);
	MaxBetButton->EnableWindow(FALSE);
	}
}
//-------------------------------------------------------------------
void TPokerDialog::ProcessMaxBetButton()
{
int currCredits;
char str[7];
	for (int i=Bet;i<=9;i++)
	{
	SoundBet();
	Bet++;
	SetRewards(PrizeList,Bet);
	RewardList->ClearList();
	FillRewardList(PrizeList,RewardList);
	Credits->GetText(str,7);
	currCredits=atoi(str);
	currCredits=currCredits-1;
	itoa(currCredits,str,10);
	Credits->Clear();
	Credits->SetText(str);
	}
	RetryButton->EnableWindow(TRUE);
	BetButton->EnableWindow(FALSE);
	MaxBetButton->EnableWindow(FALSE);
	for (int c=0;c<=52;c++)
		number[c]=0;
}
//-------------------------------------------------------------------
void TPokerDialog::ProcessRetryButton()
{
int val;
SoundRetry();
BetButton->EnableWindow(FALSE);
MaxBetButton->EnableWindow(FALSE);
Bet=0;
if ((my_flag==TRUE)&&ok)
	{
	Card1->SetProp("Value",0);
	Card2->SetProp("Value",0);
	Card3->SetProp("Value",0);
	Card4->SetProp("Value",0);
	Card5->SetProp("Value",0);
	UpdateCard(Card1);BackOn1=TRUE;
	UpdateCard(Card2);BackOn2=TRUE;
	UpdateCard(Card3);BackOn3=TRUE;
	UpdateCard(Card4);BackOn4=TRUE;
	UpdateCard(Card5);BackOn5=TRUE;
	end_of_play=FALSE;
	ok=FALSE;
	}
else
	{
	Card1->GetProp("Value",val);
	if (val==0)
		UpdateCard(Card1);
	Card2->GetProp("Value",val);
	if (val==0)
		UpdateCard(Card2);
	Card3->GetProp("Value",val);
	if (val==0)
		UpdateCard(Card3);
	Card4->GetProp("Value",val);
	if (val==0)
		UpdateCard(Card4);
	Card5->GetProp("Value",val);
	if (val==0)
		UpdateCard(Card5);
	my_flag=TRUE;
	end_of_play=TRUE;
	ok=TRUE;
	Verify_Hand(number);
	RetryButton->EnableWindow(FALSE);
	for (int l=0;l<=52;l++)
		number[l]=0;
	}
}
//-------------------------------------------------------------------
void TPokerDialog::ProcessCreditButton()
{
RedButton->ShowWindow(SW_HIDE);
BlackButton->ShowWindow(SW_HIDE);
BRCard->ShowWindow(SW_HIDE);
DoubleButton->EnableWindow(FALSE);

char str[7];
int Cred,Win;

Credits->GetText(str,7);
Cred=atoi(str);
Wins->GetText(str,7);
Win=atoi(str);
for (int c=1;c<=Win;c++)
{
SoundCredit();
itoa(Cred+c,str,10);
Credits->Clear();
Credits->SetText(str);
}
CreditButton->EnableWindow(FALSE);
BetButton->EnableWindow(TRUE);
MaxBetButton->EnableWindow(TRUE);
}
//-------------------------------------------------------------------
void TPokerDialog::ProcessDoubleButton()
{
//RedButton=new TButton(this,IDC_REDBUTTON,"&Red",130,160,10,10);
//RedButton->Create();
//BlackButton=new TButton(this,IDC_BLACKBUTTON,"&Black",130,200,10,10);
//BlackButton->Create();
//BRCard=new TVbxControl(this,IDC_MHBRCARD,"MHCD200.VBX","MhCardDeck","",30,150,30,30);
//BRCard->Create();
BRCard->SetProp("Value",0);
DoubleButton->EnableWindow(FALSE);
RedButton->ShowWindow(SW_SHOW);
BlackButton->ShowWindow(SW_SHOW);
BRCard->ShowWindow(SW_SHOW);
}
//-------------------------------------------------------------------
void TPokerDialog::ProcessRedButton()
{
int suit;
int wins;
char str[7];
BRCard->SetProp("Value",0);
Sleep(500);
UpdateCard(BRCard);
BRCard->GetProp("Suit",suit);
if (suit==1||suit==2)
{
WinSound();
Wins->GetText(str,7);
wins=atoi(str);
wins=2*wins;
itoa(wins,str,10);
Wins->Clear();
Wins->SetText(str);
}
else
{
LooseSound();
Wins->Clear();
Wins->SetText("0");
Sleep(1000);
CreditButton->EnableWindow(FALSE);
BetButton->EnableWindow(TRUE);
MaxBetButton->EnableWindow(TRUE);
RedButton->ShowWindow(SW_HIDE);
BlackButton->ShowWindow(SW_HIDE);
BRCard->ShowWindow(SW_HIDE);
}
}
//-------------------------------------------------------------------
void TPokerDialog::ProcessBlackButton()
{
int suit;
int wins;
char str[7];
BRCard->SetProp("Value",0);
Sleep(500);
UpdateCard(BRCard);
BRCard->GetProp("Suit",suit);
if (suit==0||suit==3)
{
WinSound();
Wins->GetText(str,7);
wins=atoi(str);
wins=wins*2;
itoa(wins,str,10);
Wins->Clear();
Wins->SetText(str);
}
else
{
LooseSound();
Wins->Clear();
Wins->SetText("0");
Sleep(1000);
CreditButton->EnableWindow(FALSE);
BetButton->EnableWindow(TRUE);
MaxBetButton->EnableWindow(TRUE);
RedButton->ShowWindow(SW_HIDE);
BlackButton->ShowWindow(SW_HIDE);
BRCard->ShowWindow(SW_HIDE);
}
}
//-------------------------------------------------------------------
static void UpdateWins(TStatic* wins,uint32 data)
{
char str[7];
ultoa(data,str,10);
wins->Clear();
wins->SetText(str);
}
void TPokerDialog::Verify_Hand(short* nmbr)
{
	if (RoyalFlush(nmbr)){
		WinSound();
		PrizeList->SetSelIndex(0);
		RewardList->SetSelIndex(0);
		UpdateWins(Wins,PrizeList->GetItemData(0));
		DoubleButton->EnableWindow(TRUE);
		CreditButton->EnableWindow(TRUE);}
	else if (StraightFlush(nmbr)){
		WinSound();
		PrizeList->SetSelIndex(1);
		RewardList->SetSelIndex(1);
		UpdateWins(Wins,PrizeList->GetItemData(1));
		DoubleButton->EnableWindow(TRUE);
		CreditButton->EnableWindow(TRUE);}
	else if (FourOfAKind(nmbr)){
		WinSound();
		PrizeList->SetSelIndex(2);
		RewardList->SetSelIndex(2);
		UpdateWins(Wins,PrizeList->GetItemData(2));
		DoubleButton->EnableWindow(TRUE);
		CreditButton->EnableWindow(TRUE);}
	else if (FullHouse(nmbr)){
		WinSound();
		PrizeList->SetSelIndex(3);
		RewardList->SetSelIndex(3);
		UpdateWins(Wins,PrizeList->GetItemData(3));
		DoubleButton->EnableWindow(TRUE);
		CreditButton->EnableWindow(TRUE);}
	else if (Flush(nmbr)){
		WinSound();
		PrizeList->SetSelIndex(4);
		RewardList->SetSelIndex(4);
		UpdateWins(Wins,PrizeList->GetItemData(4));
		DoubleButton->EnableWindow(TRUE);
		CreditButton->EnableWindow(TRUE);}
	else if (Straight(nmbr)){
		WinSound();
		PrizeList->SetSelIndex(5);
		RewardList->SetSelIndex(5);
		UpdateWins(Wins,PrizeList->GetItemData(5));
		DoubleButton->EnableWindow(TRUE);
		CreditButton->EnableWindow(TRUE);}
	else if (ThreeOfAKind(nmbr)){
		WinSound();
		PrizeList->SetSelIndex(6);
		RewardList->SetSelIndex(6);
		UpdateWins(Wins,PrizeList->GetItemData(6));
		DoubleButton->EnableWindow(TRUE);
		CreditButton->EnableWindow(TRUE);}
	else if (TwoPair(nmbr)){
		WinSound();
		PrizeList->SetSelIndex(7);
		RewardList->SetSelIndex(7);
		UpdateWins(Wins,PrizeList->GetItemData(7));
		DoubleButton->EnableWindow(TRUE);
		CreditButton->EnableWindow(TRUE);}
	else if (PairOfAces(nmbr)){
		WinSound();
		PrizeList->SetSelIndex(8);
		RewardList->SetSelIndex(8);
		UpdateWins(Wins,PrizeList->GetItemData(8));
		DoubleButton->EnableWindow(TRUE);
		CreditButton->EnableWindow(TRUE);}
	else {
		LooseSound();
		UpdateWins(Wins,0);
		BetButton->EnableWindow(TRUE);
		MaxBetButton->EnableWindow(TRUE);}
}
//-------------------------------------------------------------------
void TPokerDialog::UpdateCard(TVbxControl* card)
{
int val=rand()/2521+1;
int suit=rand()/8192;

if (number[13*suit+val]==0)
	{
		card->SetProp("Suit",suit);
		card->SetProp("Value",val);
		number[13*suit+val]=1;
		Sleep(100);
	}
else
	UpdateCard(card);
}
//-------------------------------------------------------------------
void TPokerDialog::EvCardMouseDown(VBXEVENT far* /*event*/)
{
	//int Button=VBX_EVENTARGNUM(event,int,0);
	//int Shift=VBX_EVENTARGNUM(event,int,1);
	//int X=VBX_EVENTARGNUM(event,int,2);
	//int Y=VBX_EVENTARGNUM(event,int,3);
}
//-------------------------------------------------------------------
void TPokerDialog::EvClickCard1(VBXEVENT far* /*event*/)
{
if (end_of_play==FALSE)
	{
	static int val;
	static int suit;
	SoundBet();
	if (BackOn1==TRUE)
		{
		Card1->GetProp("Value",val);
		Card1->GetProp("Suit",suit);
		Card1->SetProp("Value",0);
		number[13*suit+val]=2;
		my_flag=FALSE;
		}
	else
		{
		Card1->SetProp("Suit",suit);
		Card1->SetProp("Value",val);
		number[13*suit+val]=1;
		my_flag=TRUE;
		}
	BackOn1=~BackOn1;
	}
}
//-------------------------------------------------------------------
void TPokerDialog::EvClickCard2(VBXEVENT far* /*event*/)
{
if (end_of_play==FALSE)
	{
	static int val;
	static int suit;
	SoundBet();
	if (BackOn2==TRUE)
		{
		Card2->GetProp("Value",val);
		Card2->GetProp("Suit",suit);
		Card2->SetProp("Value",0);
		number[13*suit+val]=2;
		my_flag=FALSE;
		}
	else
		{
		Card2->SetProp("Suit",suit);
		Card2->SetProp("Value",val);
		number[13*suit+val]=1;
		my_flag=TRUE;
		}
	BackOn2=~BackOn2;
	}
}
//-------------------------------------------------------------------
void TPokerDialog::EvClickCard3(VBXEVENT far* /*event*/)
{
if (end_of_play==FALSE)
	{
	static int val;
	static int suit;
	SoundBet();
	if (BackOn3==TRUE)
		{
		Card3->GetProp("Value",val);
		Card3->GetProp("Suit",suit);
		Card3->SetProp("Value",0);
		number[13*suit+val]=2;
		my_flag=FALSE;
		}
	else
		{
		Card3->SetProp("Suit",suit);
		Card3->SetProp("Value",val);
		number[13*suit+val]=1;
		my_flag=TRUE;
		}
	BackOn3=~BackOn3;
	}
}
//-------------------------------------------------------------------
void TPokerDialog::EvClickCard4(VBXEVENT far* /*event*/)
{
if (end_of_play==FALSE)
	{
	static int val;
	static int suit;
	SoundBet();
	if (BackOn4==TRUE)
		{
		Card4->GetProp("Value",val);
		Card4->GetProp("Suit",suit);
		Card4->SetProp("Value",0);
		number[13*suit+val]=2;
		my_flag=FALSE;
		}
	else
		{
		Card4->SetProp("Suit",suit);
		Card4->SetProp("Value",val);
		number[13*suit+val]=1;
		my_flag=TRUE;
		}
	BackOn4=~BackOn4;
	}
}
//-------------------------------------------------------------------
void TPokerDialog::EvClickCard5(VBXEVENT far* /*event*/)
{
if (end_of_play==FALSE)
	{
	static int val;
	static int suit;
	SoundBet();
	if (BackOn5==TRUE)
		{
		Card5->GetProp("Value",val);
		Card5->GetProp("Suit",suit);
		Card5->SetProp("Value",0);
		number[13*suit+val]=2;
		my_flag=FALSE;
		}
	else
		{
		Card5->SetProp("Suit",suit);
		Card5->SetProp("Value",val);
		number[13*suit+val]=1;
		my_flag=TRUE;
		}
	BackOn5=~BackOn5;
	}
}
//-------------------------------------------------------------------
void TPokerDialog::EvClickCard6(VBXEVENT far* /*event*/)
{
static int Deck;

	Card6->GetProp("CardBack",Deck);
	Deck++;
	if (Deck>12)
		Deck=0;
	Card1->SetProp("CardBack",Deck);
	Card2->SetProp("CardBack",Deck);
	Card3->SetProp("CardBack",Deck);
	Card4->SetProp("CardBack",Deck);
	Card5->SetProp("CardBack",Deck);
	Card6->SetProp("CardBack",Deck);
	BRCard->SetProp("CardBack",Deck);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
class TPokerApp : public TApplication
{
	public:
		TPokerApp();
		~TPokerApp();
		void InitMainWindow();
	protected:
		TDialog* aboutdlg;
		TDialog* MyClientWindow;
		TFrameWindow* MyMainWindow;

  DECLARE_RESPONSE_TABLE(TPokerApp);
};
//-------------------------------------------------------------------
//-------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(TPokerApp,TApplication)
END_RESPONSE_TABLE;
//-------------------------------------------------------------------
TPokerApp::TPokerApp()
	:TApplication()
	 {
	 }
TPokerApp::~TPokerApp()
{
delete MyMainWindow;
}
//-------------------------------------------------------------------
void TPokerApp::InitMainWindow()
{
EnableBWCC();
MyClientWindow=new TPokerDialog(0,IDD_POKER);
MyMainWindow=new TFrameWindow(0, "AMSPoker for Windows 95 \
 - UNREGISTERED !",MyClientWindow,TRUE);
MyMainWindow->Attr.Style&=~(WS_MAXIMIZEBOX|WS_THICKFRAME);
MyMainWindow->AssignMenu(IDM_MENU);
MyMainWindow->SetIcon(this,IDI_POKERICON);
SetMainWindow(MyMainWindow);
aboutdlg=new TDialog(GetMainWindow(),IDD_ABOUT);
aboutdlg->Execute();
}
//-------------------------------------------------------------------
//-------------------OwlMain(int argc,char* argv[])------------------
int OwlMain(int /*argc*/,char* /*argv*/[])
{
TPokerApp* app;
int status;
int done;
TBIVbxLibrary vbxLib;// constructing this loads & inits the library

do
{
	try
	{
	app=new TPokerApp;
	status=app->Run();
	done=1;
		if (status)
		{
		char buf[40];
		wsprintf(buf,"Run returned %i",status);
		done=HandleGlobalException(xmsg(string(buf)),
						"Abnormal Termination","RunAgain?");
		}
	}
	catch (xmsg& x)
	{
	done=status=HandleGlobalException(x,
						"Abnormal Termination,uncaught xmsg","RunAgain?");
	}
	catch (...)
	{
	done=status=HandleGlobalException(xmsg(string()),
						"Abnormal Termination,uncaught ...","RunAgain?");
	}
	delete app;
}
while (!done);
return status;
}
