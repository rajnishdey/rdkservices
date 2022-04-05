# RDK Services #

RDK services are a set of JSON-RPC based RESTful services for accessing various set-top box components. RDK Services are managed and accessed through the [Thunder](https://github.com/rdkcentral/Thunder) framework. Thunder supports both HTTP and Websocket requests, making the services easily accessible to [Lightning](https://github.com/rdkcentral/Lightning), Web, and native client applications.
<br><br>

## Contributing to RDKServices ##

### **License Requirements** ###
1. Before RDK accepts your code into the project you must sign the RDK [Contributor License Agreement (CLA)](https://developer.rdkcentral.com/source/contribute/contribute/before_you_contribute/).

2. Each new file should include the latest [RDKM license header](https://developer.rdkcentral.com/source/source-code/source-code/coding_guideline/).

3. License for this project is included in the root directory and there shouldn't be any additional license file in any of the subfolders.
<br><br>

### **How to contribute?** ###
1. [Fork](https://docs.github.com/en/github/getting-started-with-github/quickstart/fork-a-repo) the repository, commit your changes, build and test it in at least one approved test platform/device.

2. To test it in a RDKV device, update SRC_URI and SRCREV in the [rdkservices_git.bb](https://gerrit.teamccp.com/plugins/gitiles/rdk/yocto_oe/layers/meta-rdk-video/+/2103_sprint/recipes-extended/rdkservices/rdkservices_git.bb) recipe to point to your fork.

3. Submit your changes as a [pull request](https://docs.github.com/en/github/collaborating-with-issues-and-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request-from-a-fork) to the latest sprint branch.

4. If more than one developer has to work on a particular feature, request for a dev branch to be created.
<br><br>

### **Pull request Checklist** ###
1. When a pull request is submitted, blackduck, copyright and cla checks will automatically be triggered. Ensure these checks have passed (turned into green).

2. At least one reviewer needs to **review and approve** the pull request.

3. For tracking and release management purposes, each pull request and all the commits in the pull request shall include **RDK ticket number(s) or Github issue number(s)** and “reason for the change”.

4. Any pull request from Comcast developers should include a link to successful gerrit verification (in the comment section).

5. To verify your changeset in gerrit, submit a test gerrit change to [rdkservices_git.bb](https://gerrit.teamccp.com/plugins/gitiles/rdk/yocto_oe/layers/meta-rdk-video/+/2103_sprint/recipes-extended/rdkservices/rdkservices_git.bb) with the SRC_URI and SRCREV pointing to your fork.

6. If the changes to RDKServices require any Thunder framework changes, the contributor has to plan for a limited regression testing (with the Thunder and RDKServices changes) before submitting the pull request.
<br><br>

## Comcast CI/CD ##

### **RDKServices branches - sprint vs main (Specific to RDKV Builds)** ###
1. Comcast gerrit sprint branch will point to rdkservices sprint branch.
    1. For example [2103_sprint](https://gerrit.teamccp.com/plugins/gitiles/rdk/yocto_oe/layers/meta-rdk-video/+/2103_sprint/recipes-extended/rdkservices/rdkservices_git.bb#11) branch gerrit recipe will point to [sprint/2103](https://github.com/rdkcentral/rdkservices/tree/sprint/2103) branch in github.

2. Comcast gerrit [stable2](https://gerrit.teamccp.com/plugins/gitiles/rdk/yocto_oe/layers/meta-rdk-video/+/stable2/recipes-extended/rdkservices/rdkservices_git.bb#11) branch will point to rdkservices [main](https://github.com/rdkcentral/rdkservices/tree/main) branch in github.
<br><br>

### **Sprint/stable2 git hash updates (Specific to RDKV Builds)** ###
1. Git hash in [rdkservices_git.bb (sprint_branch)](https://gerrit.teamccp.com/plugins/gitiles/rdk/yocto_oe/layers/meta-rdk-video/+/2103_sprint/recipes-extended/rdkservices/rdkservices_git.bb) will be updated periodically (at least once a week).
    1. Dev owner to follow up as needed and update the JIRA ticket to “ready for sprint testing”.

2. Once a change is verified in the sprint branch and approved by RM, the developer can contact the maintainers to cherry-pick the change set to the [main branch](https://github.com/rdkcentral/rdkservices/tree/main) or submit the cherry picked change set to the main branch.
    1. Git hash in [rdkservices_git.bb (stable2)](https://gerrit.teamccp.com/plugins/gitiles/rdk/yocto_oe/layers/meta-rdk-video/+/stable2/recipes-extended/rdkservices/rdkservices_git.bb) will be updated periodically.
    2. Dev owner to follow up as needed and update the ticket to “ready for release testing”.

3. What if a changeset in the sprint branch fails sprint testing?
    1. The developer has to submit a pull request to undo the commit before the end of the sprint cycle or
    2. The changeset will be abandoned in the sprint branch and won’t make into the main/stable2
<br><br>

### **Upstream Vs Patch** ###
1. Patches will increase the chances of build failures when the git hash is moved to a newer version.

2. We encourage everyone to upstream all the changes to GitHub instead of using patches

3. On a need basis, a developer can request the maintainers for an approval to use a patch in RDKV build (as a stop-gap measure). An unapproved patch will be rejected.
<br><br>

## Documentation ##

RDK services are described using [JSON Schema](https://json-schema.org/). JSON Schema provides a standard approach for describing APIs and ensures consistency across all APIs. The Thunder framework includes two schemas that are used to describe a service:

* [plugin.schema.json](https://github.com/rdkcentral/Thunder/blob/master/Tools/JsonGenerator/schemas/plugin.schema.json): A schema for defining a Thunder Plugin.
* [interface.schema.json](https://github.com/rdkcentral/Thunder/blob/master/Tools/JsonGenerator/schemas/interface.schema.json): A schema for defining the properties, methods, and events of a service.

Each RDK service has an instance of these schemas in the root of the service directory. For example, `MyServicePlugin.json` and `MyService.json`. These files are used to generate API documentation as Markdown. Each service has a Markdown file that is written to the `/doc` directory in a service folder. The following demonstrates the folder structure:

```shell
/rdkservices
    /MyService
        /MyService.json
        /MyServicePlugin.json
        /doc
            /doc/MyServicePlugin.md
```

Markdown files are generated using the Thunder [JsonGenerator](https://github.com/rdkcentral/Thunder/tree/master/Tools/JsonGenerator) tool. See the Thunder [README](https://github.com/rdkcentral/Thunder/blob/master/README.md) for documentation and requirements.

To generate the markdown:

1. Clone the Thunder repository if it is not already on your system.
2. Change directories to `Thunder/Tools/JsonGenerator`.
3. Run `JsonGenerator.py` and provide the location of the service JSON plugin file using the `--docs` argument and the output directory using the `-o` argument. You must also include the `--no-interfaces-section` argument; otherwise, an interface section is added to the markdown that links back to the ThunderInterfaces project. Make certain that you are pointing to the plugin definition and not the interface definition. Here is an example of using the JsonGenerator tool:

   `./JsonGenerator.py --docs ../../../rdkservices/MyService/MyServicePlugin.json  -o doc --no-interfaces-section --verbose $files`

   The `MyServicePlugin.md` file is written to the `/doc` folder.

Use the existing services as a guide when learning the structure of both the plugin and interface schemas.
<br><br>

## Questions? ##

If you have any questions or concerns reach out to the RDKServices maintainers - [Vijay Selvaraj](mailto:VijayAnand_Selvaraj@cable.comcast.com) / [Anand Kandasamy](mailto:anand_kandasamy@comcast.com)

For a plugin specific question, maintainers might refer you to the plugin owner(s).
<br><br>

## Coding Guidelines ##

1. **Be Consistent**

    * The point of having style guidelines is to have a common vocabulary of coding so people can concentrate on what you’re saying rather than on how you’re saying it.

    * If the code you add to a file looks drastically different from the existing code around it, it throws readers out of their rhythm. Avoid this.

    * If you’re editing code, take a few minutes to determine the coding style of the component and apply the same style.

    * To maintain uniformity in all text-editors, set TAB size to 2 or 4 spaces and replace TAB by SPACES

    * If they use spaces around all their arithmetic operators, you should too.
    
    * If the comments have little boxes of hash marks around them, make your comments have little boxes of hash marks around them too.

    * Minimise the use of exceptions and handle exceptions locally if possible
    
    * All resources acquired by a RDKSerice must be released by the Deinitialize method and/or the destructor

2. RDK services are implemented as Thunder Plugins and must adhere to the [PluginHost::IPlugin](https://github.com/rdkcentral/Thunder/blob/master/Source/plugins/IPlugin.h) interface. This interface is accessible by extending the [AbstractPlugin](https://github.com/rdkcentral/rdkservices/blob/main/helpers/AbstractPlugin.h) helper class.

3. All RDK Services must have a callsign with a prefix of `org.rdk`. RDK Service name must be CamelCase and start with a capital letter.

4. All method, parameter and event names must be camelCase and start with a lowercase letter.

5. MODULE_NAME

    * Thunder provides a trace and warning reporting feature. To accurately identify the source of a warning, Thunder needs to know the human readable name of the package (executable or library). This package name is defined by the MODULE_NAME and declared by the  MODULE_NAME_DECLARATION()

    * Any package that includes a Thunder component requires such a definition and declaration. If the definition is missing, a compiler error will be reported (error missing MODULE_NAME) and if the declaration is missing, a linker error will be reported (missing or duplicate symbol)

    * MODULE_NAME is typically found in "Module.h" and "Module.cpp"

        * Module.h
            ```
            #ifndef MODULE_NAME
            #define MODULE_NAME Plugin_IOController
            #endif
        * Module.c
            ```
            #include "Module.h"
            MODULE_NAME_DECLARATION(BUILD_REFERENCE)

6. Versioning
    
    * All RDK Services by default will start with version 1.
    
    * Any client facing changes (like Removing or Adding an API) to RDKServices should be made by incrementing the version to the next whole number (2,3,4...). This will ensure that clients using existing versions are not affected.
    
    * Use the AbstractPlugin [registerMethod](https://github.com/rdkcentral/rdkservices/blob/main/helpers/AbstractPlugin.h#L76) helper function to register APIs to specific versions. Here is an [example](https://github.com/rdkcentral/rdkservices/commit/3692632373e8e82dba92bec56f9e6082b430a829#diff-f6cd28bb8911a0253a4601f823d3777089aecd54eb214bc6cdc227961da7b13f).

7. Initialization and Cleanup

    * Prefer to do Plugin Initialization within IPlugin [Initialize()](https://github.com/rdkcentral/Thunder/blob/master/Source/plugins/IPlugin.h#L71). If there is any error in initialization return non-empty string with useful error information. This will ensure that plugin doesn't get activated and also return this error information to the caller. Ensure that any Initialization done within Initialize() gets cleaned up within IPlugin [Deinitialize()](https://github.com/rdkcentral/Thunder/blob/master/Source/plugins/IPlugin.h#L80) which gets called when the plugin is deactivated.
    
    * Ensure that any std::threads created are joined within Deinitialize() or the destructor to avoid [std::terminate](https://en.cppreference.com/w/cpp/thread/thread/~thread) exception. Use the [ThreadRAII](https://github.com/rdkcentral/rdkservices/blob/sprint/2103/helpers/utils.h#L359) class for creating threads which will ensure that the thread gets joined before destruction.