import adapter from '@sveltejs/adapter-auto';
import { vitePreprocess } from '@sveltejs/kit/vite';
import { resolve } from 'path';

/**
 * @type {import('@sveltejs/kit').Config}
 */
export default {
    preprocess: vitePreprocess(),
    kit: {
        adapter: adapter(),
        alias: {
            $components: resolve('./src/components'),
            $lib: resolve('./src/lib'),
            $routes: resolve('./src/routes'),
            $src: resolve('./src')
        }
    }
};
