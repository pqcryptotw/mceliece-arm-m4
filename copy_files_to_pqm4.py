
import string
import re
import platform
import sys
import os
import subprocess
import shutil
import datetime



def change_macro( fn , macro , on_or_off ):
  with open( fn , "r+") as f:
    lines = f.readlines()
    for i,l in enumerate(lines) :
      if macro in l :
        if( on_or_off ) :  lines[i] = "#define " + macro + "\n"
        else :  lines[i] = "//#define " + macro + "\n"
    f.seek(0)
    f.write( "".join(lines) )
    f.truncate()



my_location = os.path.realpath(os.path.join(os.getcwd(), os.path.dirname(__file__)))


if __name__ == "__main__":
  print( "tool for copy files to pqm4\n" )
  print( f"current location: {my_location}\n")

  if 1 == len(sys.argv) :
    print('Usage: \n\tpython3 copy_files_to_pqm4.py _path_pqm4_\n')
    print('\tPlease input path to PQM4.\n')
    sys.exit(0)

  pqm4_dir_name = sys.argv[1]
  if '/' == pqm4_dir_name[-1] : pqm4_dir_name = pqm4_dir_name[:-1]

  print( f"path to pqm4: {pqm4_dir_name}" );

  if not os.path.isdir( pqm4_dir_name ):
    print( f"{pqm4_dir_name} is not a directory." )
    sys.exit(0)

  src = f"{my_location}/pqm4tester"
  dest = f"{pqm4_dir_name}/mupq/crypto_kem"
  print( f"copy testers: {src} -> {dest}" )
  shutil.copytree( src , dest , dirs_exist_ok=True )
  os.remove( dest + "/ld_script_for_flash.ld" )

  src = f"{my_location}/pqm4tester/ld_script_for_flash.ld"
  dest = f"{pqm4_dir_name}/ldscripts/crypto_kem_mc348864keygen.ld"
  print( f"copy ldscript: {src}  -> {dest}" )
  os.makedirs(os.path.dirname(dest), exist_ok=True)
  shutil.copyfile( src , dest );
  #shutil.copytree( src , dest , dirs_exist_ok=True )

  src = f"{my_location}/pqm4tester/ld_script_for_flash.ld"
  dest = f"{pqm4_dir_name}/ldscripts/crypto_kem_mc348864fkeygen.ld"
  print( f"copy ldscript: {src}  -> {dest}" )
  shutil.copyfile( src , dest );


  src = f"{my_location}/u32_n3488_t64/src"
  dest = f"{pqm4_dir_name}/crypto_kem/mc348864keygen"
  print( f"copy files: {src} -> {dest}" )
  shutil.copytree( src , dest , dirs_exist_ok=True )
  src = f"{my_location}/u32_n3488_t64/stm32f4flashtool"
  print( f"copy files: {src} -> {dest}" )
  shutil.copytree( src , dest , dirs_exist_ok=True )
  ## edit run_config.h : turn on _F_PARAM_
  print( f"turn off macro _F_PARAM_ in {dest + '/run_config.h'}" )
  change_macro( dest + "/run_config.h" , "_F_PARAM_" , False )
  print( f"turn on macro _PK_IN_FLASH_ in {dest + '/run_config.h'}" )
  change_macro( dest + "/run_config.h" , "_PK_IN_FLASH_" , True )


  src = f"{my_location}/u32_n3488_t64/src"
  dest = f"{pqm4_dir_name}/crypto_kem/mc348864fkeygen"
  print( f"copy files: {src} -> {dest}" )
  shutil.copytree( src , dest , dirs_exist_ok=True )
  src = f"{my_location}/u32_n3488_t64/stm32f4flashtool"
  print( f"copy files: {src} -> {dest}" )
  shutil.copytree( src , dest , dirs_exist_ok=True )
  ## edit run_config.h : turn on _F_PARAM_
  print( f"turn on macro _F_PARAM_ in {dest + '/run_config.h'}" )
  change_macro( dest + "/run_config.h" , "_F_PARAM_" , True )
  print( f"turn on macro _PK_IN_FLASH_ in {dest + '/run_config.h'}" )
  change_macro( dest + "/run_config.h" , "_PK_IN_FLASH_" , True )


  src = f"{my_location}/u32_n3488_t64/src"
  src2 = f"{my_location}/u32_n3488_t64/testkeypair"
  dest = f"{pqm4_dir_name}/crypto_kem/mc348864"
  print( f"copy files: {src} , {src2} -> {dest}" )
  shutil.copytree( src , dest , dirs_exist_ok=True )
  shutil.copytree( src2 , dest , dirs_exist_ok=True )
  ## edit run_config.h : turn off _PK_IN_FLASH_
  print( f"turn off macro _PK_IN_FLASH_ in {dest + '/run_config.h'}" )
  change_macro( dest + "/run_config.h" , "_PK_IN_FLASH_" , False )


  src = f"{my_location}/u32_n4608_t96/src"
  src2 = f"{my_location}/u32_n4608_t96/testkeypair"
  dest = f"{pqm4_dir_name}/crypto_kem/mc460896"
  print( f"copy files: {src} , {src2} -> {dest}" )
  shutil.copytree( src , dest , dirs_exist_ok=True )
  shutil.copytree( src2 , dest , dirs_exist_ok=True )

  src = f"{my_location}/u32_n6688_t128/src"
  src2 = f"{my_location}/u32_n6688_t128/testkeypair"
  dest = f"{pqm4_dir_name}/crypto_kem/mc6688128"
  print( f"copy files: {src} , {src2} -> {dest}" )
  shutil.copytree( src , dest , dirs_exist_ok=True )
  shutil.copytree( src2 , dest , dirs_exist_ok=True )

  src = f"{my_location}/u32_n8192_t128/src"
  src2 = f"{my_location}/u32_n8192_t128/testkeypair"
  dest = f"{pqm4_dir_name}/crypto_kem/mc8192128"
  print( f"copy files: {src} , {src2} -> {dest}" )
  shutil.copytree( src , dest , dirs_exist_ok=True )
  shutil.copytree( src2 , dest , dirs_exist_ok=True )


  print( "\n\npossible test scenarios:" )
  print( "test mc348864  keygen: 'make IMPLEMENTATION_PATH=crypto_kem/mc348864keygen bin/crypto_kem_mc348864keygen_speed0.bin'" )
  print( "test mc348864f keygen: 'make IMPLEMENTATION_PATH=crypto_kem/mc348864fkeygen bin/crypto_kem_mc348864fkeygen_speed0.bin'" )

  print( "test mc348864 enc/dec: 'make IMPLEMENTATION_PATH=crypto_kem/mc348864 bin/crypto_kem_mc348864_speed1.bin'" )
  print( "test mc460896 enc/dec: 'make IMPLEMENTATION_PATH=crypto_kem/mc460896 bin/crypto_kem_mc460896_speed1.bin'" )

  print( "test mc6688128  decap: 'make IMPLEMENTATION_PATH=crypto_kem/mc6688128 bin/crypto_kem_mc6688128_speed2.bin'" )
  print( "test mc8192128  decap: 'make IMPLEMENTATION_PATH=crypto_kem/mc8192128 bin/crypto_kem_mc8192128_speed2.bin'" )

  print( "\nwrite binary file to the STM32F4 device: 'st-flash write bin/crypto_kem_XXX_speedX.bin 0x8000000'" )
  print( "\n\nenjoy." )
