int main( int argc, char *argv[] )
{
	int a = 0;
	if(argc == 1)
	{
		a = 1;
	}
	else if(argc == 2)
	{
		a = 2;
	}
	else if(argc == 3)
	{
		a = 3;
	}
	else
	{
		a = 4;
	}

	a = 5;

	if(argc == 4)
	{
		a = 1;
	}
	else if(argc == 5)
	{
		a = 2;
	}
	else if(argc == 6)
	{
		a = 3;
	}

	return 0;
}
