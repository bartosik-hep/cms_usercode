perl -pi -e "s/     src = 'AlignmentTrackSelector'/     src = 'AlignmentTrackSelector',\n     TTRHBuilder = 'WithTrackAngle'/g" $(find *mp1208_LA_cfg.py)
