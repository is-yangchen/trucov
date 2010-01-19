int foof(bool x)
{
   if (x)
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

int foobar(bool x)
{
   return 1;
}

int main()
{
   int x = foof(true); 
   
   return 0;
}
