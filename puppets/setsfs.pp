file { "managersFS":
    ensure => directory,
    path => "/home/managers",
    owner => "mscott",
    group => "managers",
    mode => 0770,
}

file { "salesFS":
    ensure => directory,
    path => "/home/sales",
    owner => "abernard",
    group => "sales",
    mode => 0770,
}

file { "accountingFS":
    ensure => directory,
    path => "/home/accounting",
    owner => "amartin",
    group => "accounting",
    mode => 0770,
}
