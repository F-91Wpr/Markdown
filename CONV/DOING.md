 

## WSL2 网络问题

在 Windows 中加一个防火墙规则:

```shell
PS C:\WINDOWS\system32>  New-NetFirewallRule -DisplayName "WSL" -Direction Inbound  -InterfaceAlias "vEthernet (WSL)"  -Action Allow
```