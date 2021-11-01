

group { "managers":
    ensure => "present",
    gid => "2500",
}

group { "accounting":
    ensure => "present",
    gid => "2501"
}

group { "sales":
    ensure => "present",
    gid => "2502",
}

group { "abernard":
    ensure => "present",
    gid => "600"
}
user { "abernard":
    ensure => present,
    uid => "600",
    gid => "600",
    groups => ["sales"],
    comment => "Andy Bernard",
    password => '$1$5hTU2iaY$6GbjPwe61BXwAq7vtANAu/',
    shell => "/bin/bash",
    home => "/home/abernard",
    managehome => true,
}
file { "abernardhome":
    ensure => directory,
    path => "/home/abernard",
    owner => "abernard",
    group => "abernard",
    mode => 0770,
}

group { "amartin":
    ensure => "present",
    gid => "601"
}
user { "amartin":
    ensure => present,
    uid => "601",
    gid => "601",
    groups => ["accounting"],
    comment => "Angela Martin",
    password => '$1$5hTU2iaY$6GbjPwe61BXwAq7vtANAu/',
    shell => "/bin/bash",
    home => "/home/amartin",
    managehome => true,
}
file { "amartinhome":
    ensure => directory,
    path => "/home/amartin",
    owner => "amartin",
    group => "amartin",
    mode => 0770,
}

group { "cbratton":
    ensure => "present",
    gid => "602"
}
user { "cbratton":
    ensure => present,
    uid => "602",
    gid => "602",
    comment => "Creed Bratton",
    password => '$1$5hTU2iaY$6GbjPwe61BXwAq7vtANAu/',
    shell => "/bin/bash",
    home => "/home/cbratton",
    managehome => true,
}
file { "cbrattonhome":
    ensure => directory,
    path => "/home/cbratton",
    owner => "cbratton",
    group => "cbratton",
    mode => 0770,
}


group { "dphilbin":
    ensure => "present",
    gid => "603"
}
user { "dphilbin":
    ensure => present,
    uid => "603",
    gid => "603",
    comment => "Darryl Philbin",
    password => '$1$5hTU2iaY$6GbjPwe61BXwAq7vtANAu/',
    shell => "/bin/bash",
    home => "/home/dphilbin",
    managehome => true,
}
file { "dphilbinhome":
    ensure => directory,
    path => "/home/dphilbin",
    owner => "dphilbin",
    group => "dphilbin",
    mode => 0770,
}

group { "dschrute":
    ensure => "present",
    gid => "604"
}
user { "dschrute":
    ensure => present,
    uid => "604",
    gid => "604",
    groups => ["managers"],
    comment => "Dwight Schrute",
    password => '$1$5hTU2iaY$6GbjPwe61BXwAq7vtANAu/',
    shell => "/bin/bash",
    home => "/home/dschrute",
    managehome => true,
}
file { "dschrutehome":
    ensure => directory,
    path => "/home/dschrute",
    owner => "dschrute",
    group => "dschrute",
    mode => 0770,
}

group { "ithomas":
    ensure => "present",
    gid => "666"
}
user { "ithomas":
    ensure => present,
    uid => "666",
    gid => "666",
    comment => "Isaiah Thomas",
    password => '$1$5hTU2iaY$6GbjPwe61BXwAq7vtANAu/',
    shell => "/bin/bash",
    home => "/home/ithomas",
    managehome => true,
}
file { "ithomashome":
    ensure => directory,
    path => "/home/ithomas",
    owner => "ithomas",
    group => "ithomas",
    mode => 0770,
}



group { "jhalpert":
    ensure => "present",
    gid => "606"
}
user { "jhalpert":
    ensure => present,
    uid => "606",
    gid => "606",
    groups => ["managers"],
    comment => "Jim Halpert",
    password => '$1$5hTU2iaY$6GbjPwe61BXwAq7vtANAu/',
    shell => "/bin/bash",
    home => "/home/jhalpert",
    managehome => true,
}
file { "jhalperthome":
    ensure => directory,
    path => "/home/jhalpert",
    owner => "jhalpert",
    group => "jhalpert",
    mode => 0770,
}

group { "kkapoor":
    ensure => "present",
    gid => "607"
}
user { "kkapoor":
    ensure => present,
    uid => "607",
    gid => "607",
    comment => "Kelly Kapoor",
    password => '$1$5hTU2iaY$6GbjPwe61BXwAq7vtANAu/',
    shell => "/bin/bash",
    home => "/home/kkapoor",
    managehome => true,
}
file { "kkapoorhome":
    ensure => directory,
    path => "/home/kkapoor",
    owner => "kkapoor",
    group => "kkapoor",
    mode => 0770,
}

group { "mscott":
    ensure => "present",
    gid => "608"
}
user { "mscott":
    ensure => present,
    uid => "608",
    gid => "608",
    groups => ["managers"],
    comment => "Michael Scott",
    password => '$1$5hTU2iaY$6GbjPwe61BXwAq7vtANAu/',
    shell => "/bin/bash",
    home => "/home/mscott",
    managehome => true,
}
file { "mscotthome":
    ensure => directory,
    path => "/home/mscott",
    owner => "mscott",
    group => "mscott",
    mode => 0770,
}

group { "mpalmer":
    ensure => "present",
    gid => "609"
}
user { "mpalmer":
    ensure => present,
    uid => "609",
    gid => "609",
    comment => "Meredith Palmer",
    password => '$1$5hTU2iaY$6GbjPwe61BXwAq7vtANAu/',
    shell => "/bin/bash",
    home => "/home/mpalmer",
    managehome => true,
}
file { "mpalmerhome":
    ensure => directory,
    path => "/home/mpalmer",
    owner => "mpalmer",
    group => "mpalmer",
    mode => 0770,
}


group { "pbeesly":
    ensure => "present",
    gid => "610"
}
user { "pbeesly":
    ensure => present,
    uid => "610",
    gid => "610",
    comment => "Pam Beesly",
    password => '$1$5hTU2iaY$6GbjPwe61BXwAq7vtANAu/',
    shell => "/bin/bash",
    home => "/home/pbeesly",
    managehome => true,
}
file { "pbeeslyhome":
    ensure => directory,
    path => "/home/pbeesly",
    owner => "pbeesly",
    group => "pbeesly",
    mode => 0770,
}



group { "omartinez":
    ensure => "present",
    gid => "611"
}
user { "omartinez":
    ensure => present,
    uid => "611",
    gid => "611",
    groups => ["accounting"],
    comment => "Oscar Martinez",
    password => '$1$5hTU2iaY$6GbjPwe61BXwAq7vtANAu/',
    shell => "/bin/bash",
    home => "/home/omartinez",
    managehome => true,
}
file { "omartinezhome":
    ensure => directory,
    path => "/home/omartinez",
    owner => "omartinez",
    group => "omartinez",
    mode => 0770,
}

group { "tflenderson":
    ensure => "present",
    gid => "612"
}
user { "tflenderson":
    ensure => present,
    uid => "612",
    gid => "612",
    comment => "Toby Flenderson",
    password => '$1$5hTU2iaY$6GbjPwe61BXwAq7vtANAu/',
    shell => "/bin/bash",
    home => "/home/tflenderson",
    managehome => true,
}
file { "tflendersonhome":
    ensure => directory,
    path => "/home/tflenderson",
    owner => "tflenderson",
    group => "tflenderson",
    mode => 0770,
}

group { "kmalone":
    ensure => "present",
    gid => "613"
}
user { "kmalone":
    ensure => present,
    uid => "613",
    gid => "613",
    groups => ["accounting"],
    comment => "Kevin Malone",
    password => '$1$5hTU2iaY$6GbjPwe61BXwAq7vtANAu/',
    shell => "/bin/bash",
    home => "/home/kmalone",
    managehome => true,
}
file { "kmalonehome":
    ensure => directory,
    path => "/home/kmalone",
    owner => "kmalone",
    group => "kmalone",
    mode => 0770,
}

group { "plapin":
    ensure => "present",
    gid => "614"
}
user { "plapin":
    ensure => present,
    uid => "614",
    gid => "614",
    groups => ["sales"],
    comment => "Phyllis Lapin",
    password => '$1$5hTU2iaY$6GbjPwe61BXwAq7vtANAu/',
    shell => "/bin/bash",
    home => "/home/plapin",
    managehome => true,
}
file { "plapinhome":
    ensure => directory,
    path => "/home/plapin",
    owner => "plapin",
    group => "plapin",
    mode => 0770,
}

group { "shudson":
    ensure => "present",
    gid => "615"
}
user { "shudson":
    ensure => present,
    uid => "615",
    gid => "615",
    groups => ["sales"],
    comment => "Stanley Hudson",
    password => '$1$5hTU2iaY$6GbjPwe61BXwAq7vtANAu/',
    shell => "/bin/bash",
    home => "/home/shudson",
    managehome => true,
}
file { "shudsonhome":
    ensure => directory,
    path => "/home/shudson",
    owner => "shudson",
    group => "shudson",
    mode => 0770,
}






































