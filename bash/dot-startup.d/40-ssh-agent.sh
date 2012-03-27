#
# Starts up the ssh agent.
#
# Originally from: http://mah.everybody.org/docs/ssh
#
# Run 
#
#   ssh-add ~/.ssh/id_dsa
#
# to enter the passphrase and register with the agent.

SSH_ENV="$HOME/.ssh/environment"

function start_agent {
     echo "Initialising new SSH agent..."
     /usr/bin/ssh-agent | sed 's/^echo/#echo/' > "${SSH_ENV}"
     echo succeeded
     chmod 600 "${SSH_ENV}"
     . "${SSH_ENV}" > /dev/null
     pubkey=~/.ssh/id-dsa-$(hostname)
     if [ -e ${pubkey} ]
     then
        /usr/bin/ssh-add ${pubkey}
     else
        /usr/bin/ssh-add 
     fi
}

# Source SSH settings, if applicable

if [ -f "${SSH_ENV}" ]; then
     . "${SSH_ENV}" > /dev/null
     #ps ${SSH_AGENT_PID} doesn't work under cywgin
     ps -ef | grep ${SSH_AGENT_PID} | grep ssh-agent$ > /dev/null || {
         start_agent;
     }
else
     start_agent;
fi 

