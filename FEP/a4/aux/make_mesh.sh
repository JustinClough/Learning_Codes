# Use this to create models and corresponding meshes

box_dir=/lore/clougj/scorec/build/test/

nx=4
ny=4
nz=0
wx=1 
wy=1 
wz=0 
quad_tri=0
model_name=quad_4.dmg
mesh_name=quad_4.smb

$box_dir/box \
  $nx \
  $ny \
  $nz \
  $wx \
  $wy \
  $wz \
  $quad_tri   \
  $mesh_name \
  $model_name 
