<?php

function dedikam_epochtodate($epoch,$format)
{
	print $epoch."\n";
        $epoch=intval($epoch);
	print $epoch."\n";
        $dt = new DateTime("@$epoch");
	print_r($dt);
    $dt->setTimezone(new DateTimeZone('Europe/Paris'));
	print_r($dt);
        return $dt->format($format);
//      return $dt->format('Y-m-d H:i:sP');
}

echo dedikam_epochtodate(1422006801,'d/m/Y')."\n";
?>
