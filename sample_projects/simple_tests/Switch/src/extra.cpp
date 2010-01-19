   // Fish may move quick or slowly.
   if ( move_fast )
   {
      fish_dart();
   }
   else
   {
      fish_float();
   }
}

//////////////////////////////////////////////////////////////////////////////
/// @brief
/// Plays an animal sound.
///
/// @param sound_type The type of sound to be played.
//////////////////////////////////////////////////////////////////////////////
void sound(int sound_type)
{
   // Create a dog to make sounds.
   Dog dog;
     
   switch( sound_type )
   {
      case 0:
         fish_blurb();
         break;
      case 1:
         dog.bark();
      case 2:
         dog.woof(); 
         break;
      default:
         break;
    }
}

