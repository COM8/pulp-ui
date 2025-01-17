# Pulp UI
GTK4 Based UI for Stats all Around AP Sensing in Böblingen using a Raspberry Pi.

## Examples
TODO

## Building

### Requirements
#### Fedora
```
sudo dnf install gtkmm4.0-devel libadwaita-devel libcurl-devel g++ clang cmake git
sudo dnf install libsoup3-devel sqlite-devel ninja-build meson
```

### Building
```
git clone https://github.com/COM8/pulp-ui.git
cd pulp-ui
mkdir build
cd build
cmake ..
cmake --build .
```

## Flatpak
The pulp-ui can be built and installed using Flatpak.

### Requirements
#### Fedora
```
sudo dnf install flatpak flatpak-builder
flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo
flatpak install flathub org.gnome.Sdk/43 org.gnome.Platform/43
```

#### Debian/Ubuntu
```
sudo apt install flatpak flatpak-builder
flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo
flatpak install flathub org.gnome.Sdk/43 org.gnome.Platform/43
```

### Building
Add `--jobs=1` to the `flatpak-builder` command when building on a Raspberry Pi to prevent running out of RAM.
```
git clone https://github.com/COM8/pulp-ui.git
cd pulp-ui
flatpak-builder --force-clean flatpak_build_dir de.pulp_ui.cli.yml
```

### Installing
```
flatpak-builder --user --install --force-clean flatpak_build_dir de.pulp_ui.cli.yml
```

### Cross-Compiling
In case you would like to cross-compile for the Raspberry Pi (aarch64), this can be done as follows:
```
sudo dnf install qemu-system-arm qemu-user-static
sudo systemctl restart systemd-binfmt.service

flatpak install flathub org.gnome.Sdk/aarch64/43 org.gnome.Platform/aarch64/43
flatpak-builder --arch=aarch64 --repo=repo --force-clean flatpak_build_dir de.pulp_ui.cli.yml
flatpak build-bundle --arch=aarch64 ./repo/ de.pulp_ui.cli.flatpak de.pulp_ui.cli

# Copy the "repo" dir to the target system
flatpak --user remote-add --no-gpg-verify pulp-ui-repo repo
flatpak --user install pulp-ui-repo de.pulp_ui.cli
```

### Uninstalling
```
flatpak uninstall de.pulp_ui.cli
```

### Executing
```
flatpak run de.pulp_ui.cli
```
