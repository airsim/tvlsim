/**
 * @class Ext.layout.container.Accordion
 * @extends Ext.layout.container.VBox
 * <p>This is a layout that manages multiple Panels in an expandable accordion style such that only
 * <b>one Panel can be expanded at any given time</b>. Each Panel has built-in support for expanding and collapsing.</p>
 * <p>Note: Only Ext.Panels <b>and all subclasses of Ext.panel.Panel</b> may be used in an accordion layout Container.</p>
 * {@img Ext.layout.container.Accordion/Ext.layout.container.Accordion.png Ext.layout.container.Accordion container layout}
 * <p>Example usage:</p>
 * <pre><code>
Ext.create('Ext.panel.Panel', {
    title: 'Accordion Layout',
    width: 300,
    height: 300,
    layout:'accordion',
    defaults: {
        // applied to each contained panel
        bodyStyle: 'padding:15px'
    },
    layoutConfig: {
        // layout-specific configs go here
        titleCollapse: false,
        animate: true,
        activeOnTop: true
    },
    items: [{
        title: 'Panel 1',
        html: 'Panel content!'
    },{
        title: 'Panel 2',
        html: 'Panel content!'
    },{
        title: 'Panel 3',
        html: 'Panel content!'
    }],
    renderTo: Ext.getBody()
});
</code></pre>
 */
Ext.define('Ext.layout.container.Accordion', {
    extend: 'Ext.layout.container.VBox',
    alias: ['layout.accordion'],
    alternateClassName: 'Ext.layout.AccordionLayout',
    
    align: 'stretch',

    /**
     * @cfg {Boolean} fill
     * True to adjust the active item's height to fill the available space in the container, false to use the
     * item's current height, or auto height if not explicitly set (defaults to true).
     */
    fill : true,
    /**
     * @cfg {Boolean} autoWidth
     * <p><b>This config is ignored in ExtJS 4.x.</b></p>
     * Child Panels have their width actively managed to fit within the accordion's width.
     */
    autoWidth : true,
    /**
     * @cfg {Boolean} titleCollapse
     * <p><b>Not implemented in PR2.</b></p>
     * True to allow expand/collapse of each contained panel by clicking anywhere on the title bar, false to allow
     * expand/collapse only when the toggle tool button is clicked (defaults to true).  When set to false,
     * {@link #hideCollapseTool} should be false also.
     */
    titleCollapse : true,
    /**
     * @cfg {Boolean} hideCollapseTool
     * True to hide the contained Panels' collapse/expand toggle buttons, false to display them (defaults to false).
     * When set to true, {@link #titleCollapse} is automatically set to <code>true</code>.
     */
    hideCollapseTool : false,
    /**
     * @cfg {Boolean} collapseFirst
     * True to make sure the collapse/expand toggle button always renders first (to the left of) any other tools
     * in the contained Panels' title bars, false to render it last (defaults to false).
     */
    collapseFirst : false,
    /**
     * @cfg {Boolean} animate
     * True to slide the contained panels open and closed during expand/collapse using animation, false to open and
     * close directly with no animation (defaults to <code>true</code>). Note: The layout performs animated collapsing
     * and expanding, <i>not</i> the child Panels.
     */
    animate : true,
    /**
     * @cfg {Boolean} activeOnTop
     * <p><b>Not implemented in PR4.</b></p>
     * <p>Only valid when {@link #multi" is <code>false</code>.</p>
     * True to swap the position of each panel as it is expanded so that it becomes the first item in the container,
     * false to keep the panels in the rendered order. <b>This is NOT compatible with "animate:true"</b> (defaults to false).
     */
    activeOnTop : false,
    /**
     * @cfg {Boolean} multi
     * Defaults to <code>false</code>. Set to <code>true</code> to enable multiple accordion items to be open at once.
     */
    multi: false,

    constructor: function() {
        var me = this;

        me.callParent(arguments);

        // animate flag must be false during initial render phase so we don't get animations.
        me.initialAnimate = me.animate;
        me.animate = false;

        // Child Panels are not absolutely positioned if we are not filling, so use a different itemCls.
        if (me.fill === false) {
            me.itemCls = Ext.baseCSSPrefix + 'accordion-item';
        }
    },

    // Cannot lay out a fitting accordion before we have been allocated a height.
    // So during render phase, layout will not be performed.
    beforeLayout: function() {
        var me = this;

        me.callParent(arguments);
        if (me.fill) {
            if (!me.owner.el.dom.style.height) {
                return false;
            }
        } else {
            me.owner.componentLayout.monitorChildren = false;
            me.autoSize = true;
            me.owner.setAutoScroll(true);
        }
    },

    renderItems : function(items, target) {
        var me = this,
            ln = items.length,
            i = 0,
            comp,
            targetSize = me.getLayoutTargetSize(),
            renderedPanels = [],
            border;

        for (; i < ln; i++) {
            comp = items[i];
            if (!comp.rendered) {
                renderedPanels.push(comp);

                // Set up initial properties for Panels in an accordion.
                if (me.collapseFirst) {
                    comp.collapseFirst = me.collapseFirst;
                }
                if (me.hideCollapseTool) {
                    comp.hideCollapseTool = me.hideCollapseTool;
                    comp.titleCollapse = true;
                }
                else if (me.titleCollapse) {
                    comp.titleCollapse = me.titleCollapse;
                }

                delete comp.hideHeader;
                comp.collapsible = true;
                comp.title = comp.title || '&#160;';
                comp.setBorder(false);

                // Set initial sizes
                comp.width = targetSize.width;
                if (me.fill) {
                    delete comp.height;
                    delete comp.flex;

                    // If there is an expanded item, all others must be rendered collapsed.
                    if (me.expandedItem !== undefined) {
                        comp.collapsed = true;
                    }
                    // Otherwise expand the first item with collapsed explicitly configured as false
                    else if (comp.collapsed === false) {
                        comp.flex = 1;
                        me.expandedItem = i;
                    } else {
                        comp.collapsed = true;
                    }
                } else {
                    delete comp.flex;
                    comp.animCollapse = me.initialAnimate;
                    comp.autoHeight = true;
                    comp.autoScroll = false;
                }
            }
        }

        // If no collapsed:false Panels found, make the first one expanded.
        if (ln && me.expandedItem === undefined) {
            me.expandedItem = 0;
            comp = items[0];
            comp.collapsed = false;
            if (me.fill) {
                comp.flex = 1;
            }
        }
        
        // Render all Panels.
        me.callParent(arguments);
                
        // Postprocess rendered Panels.
        ln = renderedPanels.length;
        for (i = 0; i < ln; i++) {
            comp = renderedPanels[i];

            // Delete the dimension property so that our align: 'stretch' processing manages the width from here
            delete comp.width;

            comp.header.addCls(Ext.baseCSSPrefix + 'accordion-hd');
            comp.body.addCls(Ext.baseCSSPrefix + 'accordion-body');
            
            // If we are fitting, then intercept expand/collapse requests. 
            if (me.fill) {
                me.owner.mon(comp, {
                    show: me.onComponentShow,
                    beforeexpand: me.onComponentExpand,
                    beforecollapse: me.onComponentCollapse,
                    scope: me
                });
            }
        }
    },

    onLayout: function() {
        var me = this;
        
        me.updatePanelClasses();
                
        if (me.fill) {
            me.callParent(arguments);
        } else {
            var targetSize = me.getLayoutTargetSize(),
                items = me.getVisibleItems(),
                len = items.length,
                i = 0, comp;

            for (; i < len; i++) {
                comp = items[i];
                if (comp.collapsed) {
                    items[i].setWidth(targetSize.width);
                } else {
                    items[i].setSize(null, null);
                }
            }
        }
        
        return me;
    },
    
    updatePanelClasses: function() {
        var children = this.getLayoutItems(),
            ln = children.length,
            siblingCollapsed = true,
            i, child;
            
        for (i = 0; i < ln; i++) {
            child = children[i];
            if (!siblingCollapsed) {
                child.header.addCls(Ext.baseCSSPrefix + 'accordion-hd-sibling-expanded');
            }
            else {
                child.header.removeCls(Ext.baseCSSPrefix + 'accordion-hd-sibling-expanded');
            }
            if (i + 1 == ln && child.collapsed) {
                child.header.addCls(Ext.baseCSSPrefix + 'accordion-hd-last-collapsed');
            }
            else {
                child.header.removeCls(Ext.baseCSSPrefix + 'accordion-hd-last-collapsed');
            }
            siblingCollapsed = child.collapsed;
        }
    },

    // When a Component expands, adjust the heights of the other Components to be just enough to accommodate
    // their headers.
    // The expanded Component receives the only flex value, and so gets all remaining space.
    onComponentExpand: function(toExpand) {
        var me = this,
            it = me.owner.items.items,
            len = it.length,
            i = 0,
            comp;

        for (; i < len; i++) {
            comp = it[i];
            if (comp === toExpand && comp.collapsed) {
                me.setExpanded(comp);
            } else if (!me.multi && (comp.rendered && comp.header.rendered && comp !== toExpand && !comp.collapsed)) {
                me.setCollapsed(comp);
            }
        }
        
        me.animate = me.initialAnimate;
        me.layout();
        me.animate = false;
        return false;
    },

    onComponentCollapse: function(comp) {
        var me = this,
            toExpand = comp.next() || comp.prev(),
            expanded = me.multi ? me.owner.query('>panel:not([collapsed])') : [];

        // If we are allowing multi, and the "toCollapse" component is NOT the only expanded Component,
        // then ask the box layout to collapse it to its header.
        if (me.multi) {
            me.setCollapsed(comp);

            // If the collapsing Panel is the only expanded one, expand the following Component.
            // All this is handling fill: true, so there must be at least one expanded,
            if (expanded.length === 1 && expanded[0] === comp) {
                me.setExpanded(toExpand);
            }
            
            me.animate = me.initialAnimate;
            me.layout();
            me.animate = false;
        }
        // Not allowing multi: expand the next sibling if possible, prev sibling if we collapsed the last
        else if (toExpand) {
            me.onComponentExpand(toExpand);
        }
        return false;
    },

    onComponentShow: function(comp) {
        // Showing a Component means that you want to see it, so expand it.
        this.onComponentExpand(comp);
    },

    setCollapsed: function(comp) {
        var otherDocks = comp.getDockedItems(),
            dockItem,
            len = otherDocks.length,
            i = 0;

        // Hide all docked items except the header
        comp.hiddenDocked = [];
        for (; i < len; i++) {
            dockItem = otherDocks[i];
            if ((dockItem !== comp.header) && !dockItem.hidden) {
                dockItem.hidden = true;
                comp.hiddenDocked.push(dockItem);
            }
        }
        comp.addCls(comp.collapsedCls);
        comp.header.addCls(comp.collapsedHeaderCls);
        comp.height = comp.header.getHeight();
        comp.el.setHeight(comp.height);
        comp.collapsed = true;
        delete comp.flex;
        comp.fireEvent('collapse', comp);
        if (comp.collapseTool) {
            comp.collapseTool.setType('expand-' + comp.getOppositeDirection(comp.collapseDirection));
        }
    },

    setExpanded: function(comp) {
        var otherDocks = comp.hiddenDocked,
            len = otherDocks ? otherDocks.length : 0,
            i = 0;

        // Show temporarily hidden docked items
        for (; i < len; i++) {
            otherDocks[i].show();
        }

        // If it was an initial native collapse which hides the body
        if (!comp.body.isVisible()) {
            comp.body.show();
        }
        delete comp.collapsed;
        delete comp.height;
        delete comp.componentLayout.lastComponentSize;
        comp.suspendLayout = false;
        comp.flex = 1;
        comp.removeCls(comp.collapsedCls);
        comp.header.removeCls(comp.collapsedHeaderCls);
        comp.fireEvent('expand', comp);
        if (comp.collapseTool) {
            comp.collapseTool.setType('collapse-' + comp.collapseDirection);
        }
        comp.setAutoScroll(comp.initialConfig.autoScroll);
    }
});