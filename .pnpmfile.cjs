/**
 * This files converts path such as bazel://my/pkg into a link under the bazel-bin directory such as link:./bazel-bin/my/pkg/pkg. We assume
 * that any dependencies using the bazel protocol are absolute, are npm_package targets, they contain a valid package.json and are local to
 * this workspace
 */

const path = require('path');
const BINDIR = "bazel-bin";
const BAZEL_PROTOCOL = "bazel:";
const LINK_PROTOCOL = "link:";

/**
 * @typedef Package
 * @property {Object.<string, string>} [dependencies]
 * @property {Object.<string, string>} [devDependencies]
 */

/**
 * Converts any bazel: into link: into the bazel-bin directory
 * @param {Package} pkg 
 * @param {*} ctx 
 * @returns {Package}
*/
function readPackage(pkg, ctx) {
    return {
        ...pkg,
        dependencies: pkg.dependencies && fixDependencies(pkg.dependencies, ctx),
        devDependencies: pkg.devDependencies && fixDependencies(pkg.devDependencies, ctx),
    };
}

/**
 * 
 * @param {Object.<string, string>} deps 
 * @param {*} ctx 
 * @returns 
 */
function fixDependencies(deps, ctx) {
    const entries = Object.entries(deps).map(([key, value]) => [
        key,
        value.indexOf(BAZEL_PROTOCOL) === 0
            ? resolveBazelRelativePath(value.substring(BAZEL_PROTOCOL.length), ctx)
            : value
    ]);

    return Object.fromEntries(entries);
}

/**
 * 
 * @param {string} label 
 * @param {*} ctx 
 * @returns 
 */
function resolveBazelRelativePath(label, ctx) {
    const normalizedLabel = normalize(label);

    let dirs;

    if (normalizedLabel.indexOf('//') == 0) {
        dirs = normalizedLabel.substring(2).split(/[/:]/);
    } else {
        const m = /^@(.*?)\/\/(.*)$/.exec(normalizedLabel);

        if (!m) {
            throw new Error(`Could not match label ${normalizedLabel}`);
        }

        dirs = ["external", m[1], ...m[2].split(/[/:]/)];
    }

    return 'link:' + path.resolve(__dirname, BINDIR, ...dirs);
}

/**
 * 
 * @param {string} label 
 * @returns {string}
 */
function normalize(label) {
    const parts = label.split(/(\/{1,2}|:)/);

    if (parts.length === 1) { // Something like @pnpm -> @pnpm//:pnpm 
        return [...parts, '//', ':', parts[0].substring(1)].join('');
    } else if (parts[parts.length - 2] != ':') { // Something like //some/pkg -> //some/pkg:pkg
        return [...parts, ':', parts[parts.length - 1]].join('');
    }

    // Something like //some/pkg:target or //:target
    return parts.join('');
}

/**
 * Web need to remove any references to the bazel bin dir in the lockfile because 
 * @param {import('@pnpm/lockfile-types').Lockfile} lockfile 
 * @param {*} ctx 
 * @returns {import('@pnpm/lockfile-types').Lockfile}
 */
function afterAllResolved(lockfile, ctx) {
    const importers = Object.entries(lockfile.importers).map(([name, importer]) => [name, removeBazelRefFromImporter(importer, ctx)]);

    return {
        ...lockfile,
        importers: Object.fromEntries(importers),
    };
}

/**
 * Removes any dependencies/devDependencies linking into the bazel-bin directory for an importer
 * @param {import('@pnpm/lockfile-types').ProjectSnapshot} importer 
 * @param {*} ctx 
 * @returns {import('@pnpm/lockfile-types').ProjectSnapshot}
 */
function removeBazelRefFromImporter(importer, ctx) {
    const specifiers = Object.fromEntries(Object.entries(importer.specifiers).filter(([name, specifier]) => !isBazelBinLink(specifier)));
    const dependencies = Object.fromEntries(Object.entries(importer.dependencies ?? {}).filter(([name, version]) => name in specifiers));
    const devDependencies = Object.fromEntries(Object.entries(importer.devDependencies ?? {}).filter(([name, version]) => name in specifiers));

    return {
        ...importer,
        specifiers,
        dependencies,
        devDependencies,
    }
}

/**
 * Checks if it is a link in the bazel-bin directory
 * @param {string} specifier 
 * @returns {boolean}
 */
function isBazelBinLink(specifier) {
    if (specifier.indexOf(LINK_PROTOCOL) !== 0) {
        return false;
    }
    const link = specifier.substring(LINK_PROTOCOL.length);

    if (!path.isAbsolute(link)) {
        return false;
    }

    const bindir = path.resolve(__dirname, BINDIR);
    const relative = path.relative(bindir, link);

    if (relative && !relative.startsWith('..') && !path.isAbsolute(relative)) {
        return true;
    }

    return false;
}

module.exports = { hooks: { readPackage, afterAllResolved, } };
