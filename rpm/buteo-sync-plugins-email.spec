Name:       buteo-sync-plugins-email
Summary:    Syncs email accounts
Version:    0.0.1
Release:    1
Group:      System/Applications
License:    TBD
URL:        https://github.com/nemomobile/buteo-sync-plugins-email
Source0:    %{name}-%{version}.tar.bz2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Network)
BuildRequires:  pkgconfig(qmfclient5)
BuildRequires:  nemo-qml-plugin-email-qt5-devel >= 0.0.20
BuildRequires:  buteo-syncfw-qt5-devel >= 0.6.10
Requires: buteo-syncfw-qt5-msyncd

%description
A plugin which provides data synchronization for email accounts.

%files
%defattr(-,root,root,-)
/usr/lib/buteo-plugins-qt5/libsyncemail-client.so
%config %{_sysconfdir}/buteo/profiles/client/*.xml
%config %{_sysconfdir}/buteo/profiles/sync/*.xml

%prep
%setup -q -n %{name}-%{version}

%build
%qmake5
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%qmake5_install
