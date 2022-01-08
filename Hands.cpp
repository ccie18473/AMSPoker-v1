#include <windows.h>
BOOL RoyalFlush(short* nmbr)
{
for (int i=0;i<4;i++)
{
if ((nmbr[13*i+1]==1)&&(nmbr[13*i+13]==1)&&(nmbr[13*i+12]==1)&&(nmbr[13*i+11]==1)&&(nmbr[13*i+10]==1))
	return TRUE;
}
return FALSE;
}
BOOL StraightFlush(short* nmbr)
{
for (int j=0;j<4;j++)
{
	for (int k=1;k<=9;k++)
	{
	if ((nmbr[j*13+k]==1)&&(nmbr[j*13+k+1]==1)&&(nmbr[j*13+k+2]==1)
		&&(nmbr[j*13+k+3]==1)&&(nmbr[j*13+k+4]==1))
		return TRUE;
	}
}
return FALSE;
}
BOOL FourOfAKind(short* nmbr)
{
for (int i=1;i<14;i++)
{
if ((nmbr[i]==1)&&(nmbr[i+13]==1)&&(nmbr[i+26]==1)&&(nmbr[i+39]==1))
	return TRUE;
}
return FALSE;
}
BOOL FullHouse(short* nmbr)
{
BOOL trio=FALSE;
BOOL pair=FALSE;
int a,b,c;

for (int i=1;i<14;i++)
{
	if ((nmbr[i]==1)&&(nmbr[i+13]==1)&&(nmbr[i+26]==1))
	{
	trio=TRUE;nmbr[i]=0;nmbr[i+13]=0;nmbr[i+26]=0;
	a=i;b=i+13;c=i+26;
	}
	else if ((nmbr[i]==1)&&(nmbr[i+13]==1)&&(nmbr[i+39]==1))
	{
	trio=TRUE;nmbr[i]=0;nmbr[i+13]=0;nmbr[i+39]=0;
	a=i;b=i+13;c=i+39;
	}
	else if ((nmbr[i]==1)&&(nmbr[i+26]==1)&&(nmbr[i+39]==1))
	{
	trio=TRUE;nmbr[i]=0;nmbr[i+26]=0;nmbr[i+39]=0;
	a=i;b=i+26;c=i+39;
	}
	else if ((nmbr[i+13]==1)&&(nmbr[i+26]==1)&&(nmbr[i+39]==1))
	{
	trio=TRUE;nmbr[i+13]=0;nmbr[i+26]=0;nmbr[i+39]=0;
	a=i+13;b=i+26;c=i+39;
	}
}
for (int j=1;j<14;j++)
{
	if ((nmbr[j]==1)&&(nmbr[j+13]==1))
	pair=TRUE;
	else if ((nmbr[j]==1)&&(nmbr[j+26]==1))
	pair=TRUE;
	else if ((nmbr[j]==1)&&(nmbr[j+39]==1))
	pair=TRUE;
	else if ((nmbr[j+13]==1)&&(nmbr[j+26]==1))
	pair=TRUE;
	else if ((nmbr[j+13]==1)&&(nmbr[j+39]==1))
	pair=TRUE;
	else if ((nmbr[j+26]==1)&&(nmbr[j+39]==1))
	pair=TRUE;
}
if (trio)
{
	nmbr[a]=1;
	nmbr[b]=1;
	nmbr[c]=1;
}
if (trio&&pair)
	return TRUE;
else
	return FALSE;
}
BOOL Flush(short* nmbr)
{
int count=0;
for (int j=0;j<4;j++)
{
	for (int k=1;k<14;k++)
	{
		if (nmbr[13*j+k]==1)
			count++;
	}
	if (count==5)
		return TRUE;
	else
		count=0;
}
return FALSE;
}
BOOL Straight(short* nmbr)
{
int i=1;
int temp;
int array[6];

for (int j=1;j<=52;j++)
{
	if (nmbr[j]==1)
	{
	array[i]=j;
	i++;
	}
}
for (int k=1;k<=5;k++)
{
	array[k]=array[k]%13;
	if (array[k]==0)
		array[k]=13;
}
for (int a=1;a<=4;a++)
{
	for (int b=a+1;b<=5;b++)
		if (array[b]<array[a])
		{
			temp=array[a];
			array[a]=array[b];
			array[b]=temp;
		}
}
if (	(array[5]==array[4]+1)&&(array[4]==array[3]+1)&&
		(array[3]==array[2]+1)&&(array[2]==array[1]+1)	)
	 return TRUE;
if (	(array[5]==13)&&(array[4]==12)&&(array[3]==11)&&
		(array[2]==10)&&(array[1]==1)	)
	 return TRUE;
return FALSE;
}
BOOL ThreeOfAKind(short* nmbr)
{
for (int i=1;i<14;i++)
{
if (	((nmbr[i]==1)&&(nmbr[i+13]==1)&&(nmbr[i+26]==1))||
		((nmbr[i]==1)&&(nmbr[i+13]==1)&&(nmbr[i+39]==1))||
		((nmbr[i]==1)&&(nmbr[i+26]==1)&&(nmbr[i+39]==1))||
		((nmbr[i+13]==1)&&(nmbr[i+26]==1)&&(nmbr[i+39]==1))  )
		return TRUE;
}
return FALSE;
}
BOOL TwoPair(short* nmbr)
{
int count=0;

for (int i=1;i<14;i++)
{
	if ((nmbr[i]==1)&&(nmbr[i+13]==1))
	count++;
	else if ((nmbr[i]==1)&&(nmbr[i+26]==1))
	count++;
	else if ((nmbr[i]==1)&&(nmbr[i+39]==1))
	count++;
	else if ((nmbr[i+13]==1)&&(nmbr[i+26]==1))
	count++;
	else if ((nmbr[i+13]==1)&&(nmbr[i+39]==1))
	count++;
	else if ((nmbr[i+26]==1)&&(nmbr[i+39]==1))
	count++;
}
if (count==2)
	return TRUE;
else
	return FALSE;
}
BOOL PairOfAces(short* nmbr)
{
if ((nmbr[1]==1)&&(nmbr[14]==1))
	return TRUE;
else if ((nmbr[1]==1)&&(nmbr[27]==1))
	return TRUE;
else if ((nmbr[1]==1)&&(nmbr[40]==1))
	return TRUE;
else if ((nmbr[14]==1)&&(nmbr[27]==1))
	return TRUE;
else if ((nmbr[14]==1)&&(nmbr[40]==1))
	return TRUE;
else if ((nmbr[27]==1)&&(nmbr[40]==1))
	return TRUE;
else return FALSE;
}
void SoundBet(void)
{
sndPlaySound("bet.wav",SND_ASYNC);
}
void SoundRetry(void)
{
sndPlaySound("retry.wav",SND_ASYNC);
}
void LooseSound(void)
{
sndPlaySound("loose.wav",SND_ASYNC);
}
void WinSound(void)
{
sndPlaySound("win.wav",SND_ASYNC);
}
void SoundCredit(void)
{
sndPlaySound("credit.wav",SND_ASYNC);
}

