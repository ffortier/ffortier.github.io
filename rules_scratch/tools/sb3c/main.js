import JSZip from 'jszip';
import parser from 'scratch-parser';
import fs from 'node:fs';
import assert from 'node:assert';
import path from 'node:path';
import util from 'node:util';

class Args {
    /**
     * Parses program arguments
     * @param {string[]} args 
     */
    static parseArgs(args = process.argv.slice(2)) {
        /** @type {string} */
        let output = '';
        /** @type {string[]} */
        let deps = [];
        /** @type {string[]} */
        let assets = [];
        /** @type {string | null} */
        let projectJson = null;

        for (let i = 0; i < args.length; i++) {
            switch (args[i]) {
                case '-o': case '--output': output = args[++i]; break;
                case '-a': case '--asset': assets.push(args[++i]); break;
                case '-d': case '--dep': deps.push(args[++i]); break;
                case '-p': case '--project_json': projectJson = args[++i]; break;
                default: throw new Error(`Unexpected argument ${args[i]} at ${i}`);
            }
        }

        assert(!!output, "output expected");

        return new Args(output, assets, deps, projectJson);
    }

    /**
     * @type {string}
     */
    output;

    /**
     * @type {string[]}
     */
    assets;

    /**
     * @type {string[]}
     */
    deps;

    /**
     * @type {string | null}
     */
    projectJson;

    /**
     * 
     * @param {string} output 
     * @param {string[]} assets 
     * @param {string[]} deps 
     * @param {string | null} projectJson 
     */
    constructor(
        output,
        assets,
        deps,
        projectJson,
    ) {
        this.output = output;
        this.assets = assets;
        this.deps = deps;
        this.projectJson = projectJson;
    }
}

const execroot = process.env.JS_BINARY__EXECROOT ?? '.';
const args = Args.parseArgs();
const sb3a = new JSZip();
const output = path.resolve(execroot, args.output);

fs.mkdirSync(path.dirname(output), { recursive: true });

const project = {
    /** @type {any[]} */
    targets: [],
    /** @type {any[]} */
    extensions: [],
    /** @type {any[]} */
    monitors: [],
    meta: {
        semver: "3.0.0",
        vm: "2.0.5",
        agent: "rules_scratch"
    }
};

for (const asset of args.assets) {
    sb3a.file(path.basename(asset), fs.createReadStream(path.resolve(execroot, asset)));
}

for (const dep of args.deps) {
    await mergeStaticLib(path.resolve(execroot, dep), project, sb3a);
}

if (args.projectJson) {
    const other = JSON.parse(fs.readFileSync(path.resolve(execroot, args.projectJson), 'utf8'));

    project.targets.push(...other.targets ?? []);
    project.extensions.push(...other.extensions ?? []);
    project.monitors.push(...other.monitors ?? []);
}

const stageIndex = project.targets.findIndex(target => target.isStage);
console.log(`output=${args.output}, targetCount=${project.targets.length}, stageIndex=${stageIndex}`)

if (stageIndex >= 1) {
    const [stage] = project.targets.splice(stageIndex, 1);

    project.targets.unshift(stage);
}
console.log(`output=${args.output}, targetCount=${project.targets.length}`)

if (path.extname(args.output) === '.sb3') {
    await link(project);
}

sb3a.file('project.json', JSON.stringify(project));

sb3a
    .generateNodeStream({ streamFiles: true })
    .pipe(fs.createWriteStream(output), { end: true });


/**
 * 
 * @param {string} staticlib 
 * @param {any} project 
 * @param {JSZip} sb3a 
 */
async function mergeStaticLib(staticlib, project, sb3a) {
    const other = await new JSZip().loadAsync(fs.readFileSync(staticlib));

    for (const [name, entry] of Object.entries(other.files)) {
        if (name === 'project.json') {
            const otherProject = JSON.parse(await entry.async('string'));

            project.targets.push(...otherProject.targets ?? []);
            project.extensions.push(...otherProject.extensions ?? []);
            project.monitors.push(...otherProject.monitors ?? []);
        } else {
            sb3a.file(name, entry.nodeStream());
        }
    }
}

async function link(project) {
    await util.promisify(parser)(JSON.stringify(project), false);
}