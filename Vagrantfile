# -*- mode: ruby -*-
# vi: set ft=ruby :

$INSTALL_BASE = <<SCRIPT
  export DEBIAN_FRONTEND=noninteractive
  apt-get update
  apt-get -y upgrade
  apt-get -y install build-essential vim-nox emacs xauth expect
  apt-get -y install git python3-dev python3-pip flex bison traceroute libbz2-dev libssl-dev
  apt-get -y install libzeroc-ice-dev libboost-all-dev

  mkdir -p /opt
  git clone https://github.com/mininet/mininet.git /opt/mininet
  pushd /opt/mininet
  git reset --hard afdf9fd
  util/install.sh -fnv
  popd

  rm -Rf /opt/pox
  mkdir -p /opt/pox
  # Install POX controller
  git clone -b gar-experimental https://github.com/noxrepo/pox.git /opt/pox

  # Install packet redirector to simpler router and run it as a service
  git clone https://github.com/eric135/cs118-connector.git /opt/cs118-connector
  pip3 install /opt/cs118-connector
  cp /vagrant/pox.service /etc/systemd/system/
  systemctl daemon-reload
  systemctl enable pox.service
  systemctl start pox.service
SCRIPT

Vagrant.configure(2) do |config|
  config.vm.box = "bento/ubuntu-20.04"

  config.vm.provider "virtualbox" do |vb|
  #   # Display the VirtualBox GUI when booting the machine
  #   vb.gui = true
  #
    # Customize the amount of memory on the VM:
    vb.memory = "1024"
  end

  config.vm.provision "shell", privileged: true, inline: $INSTALL_BASE
end


