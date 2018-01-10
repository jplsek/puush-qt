function Component()
{
    installer.installationFinished.connect(this, Component.prototype.installationFinishedPageIsShown);
    installer.finishButtonClicked.connect(this, Component.prototype.installationFinished);
}

Component.prototype.createOperations = function()
{
    component.createOperations();
}

Component.prototype.installationFinishedPageIsShown = function()
{
    try {
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
            installer.addWizardPageItem( component, "FinishForm", QInstaller.InstallationFinished );
        }
    } catch(e) {
        console.log(e);
    }
}

Component.prototype.installationFinished = function()
{
    try {
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
            var openCheckBoxChecked = component.userInterface("FinishForm").openCheckBox.checked;
            if (openCheckBoxChecked) {
                QDesktopServices.openUrl("file:///" + installer.value("TargetDir") + "/puush-qt.exe");
            }
			
			// the addOperation returns true, but does not do anything. However, running it in createOperations does work.
			// So I'm gonna keep this disabled for now unless someone figures it out, because this can be set up in puush-qt
			// itself (eventually), or done by the user manually.
            //var startupCheckBoxChecked = component.userInterface("FinishForm").startupCheckBox.checked;
            //if (startupCheckBoxChecked) {
			//	component.addOperation("CreateShortcut", "@TargetDir@/puush-qt.exe", "@UserStartMenuProgramsPath@/Startup/puush-qt.lnk");
            //} 
        }
    } catch(e) {
        console.log(e);
    }
}

